// Copyright (c) 2004 E.J. Pronk and R.E.W. van Beusekom
//
// This file is part of Nmdepend.
// 
// Nmdepend is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// Nmdepend is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Nmdepend; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <iostream>
#include <algorithm>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "Graph.h"
#include "Callback.h"
#include "ObjectPackage.h"
#include "ObjectFile.h"

namespace fs = boost::filesystem;
using namespace std;

template<class T>
class CallBackDummy : public Callback<T>
{
public:
  typedef T* Ptr;

  typedef std::pair<Ptr, Ptr> pair;
  virtual void operator()(T& from,T& to)
  {
    cout << from << " -> " << to << endl;
  }
};

template<class T>
struct wrapper
{
  wrapper(vector<T*>& v) : m_v(v) {}
  const string& operator[](int i) const
  {
    return m_v[i]->Name();
  }
  const vector<T*>& m_v;
};

struct sample_graph_writer
{
  void operator()(std::ostream& out) const
  {
    out << "graph [bgcolor=lightgrey]" << std::endl;
    out << "node [shape=circle color=white]" << std::endl;
    out << "edge [style=dashed]" << std::endl;
  }
};

class Analyser
{
public:
   typedef std::vector<fs::path> filelist_t;

   // todo : match wildcards with filter_iterator and regex (*.o) (*.a) (*.lib)
   void find_file( const fs::path& dir_path)
   {
      if ( !fs::exists( dir_path ) )
      {
         std::cout << "doesn't exist " << dir_path.native_directory_string() << std::endl;
      }

      fs::directory_iterator end_itr;
      for ( fs::directory_iterator itr( dir_path );
      itr != end_itr;
      ++itr )
      {
         if ( fs::is_directory( *itr ) )
         {
            find_file(*itr);
         }
         else
         {
            if (itr->leaf().rfind(".o") != std::string::npos)
            {
               list.push_back(*itr);
            }
         }
      }
   }

   void ReadObjects()
   {
       // temporary
      for(filelist_t::iterator pos = list.begin(); pos != list.end(); ++pos)
      {
         //todo define packages as collection of object files
         //using config setting
         fs::path::iterator p = pos->end();
         --p; //file
         std::string name = *p;
         --p; //directory containing file -> Release or Debug using msvc
         //--p; // 1 level higher for visual studio
         std::string packagename = *p;
         
         cout << pos->string() << endl;
         ObjectFile* o = new ObjectFile(m_Graph, name, m_symbols);

         std::pair<std::set<ObjectPackage>::iterator,bool> status =
              packages.insert(ObjectPackage(m_Graph2, packagename));

         ObjectPackage* op = const_cast<ObjectPackage*>(&(*(status.first)));
         o->Read(*pos);
         o->SetParent(*op);
         m_ObjectFiles.push_back(o);
      }
      
      for(std::set<ObjectPackage>::iterator i = packages.begin(); i != packages.end(); ++i)
      {
        m_packages.push_back(const_cast<ObjectPackage*>(&(*i)));
      }
   }
   void Link()
   {
      m_Graph.init(m_ObjectFiles);
      m_Graph2.init(m_packages);
      for (std::vector<ObjectFile*>::iterator pos = m_ObjectFiles.begin();
      pos != m_ObjectFiles.end();
      ++pos)
      {
         std::cout << "linking obj " << (*pos)->Name() << std::endl;
         (*pos)->Link();
      }

      wrapper<ObjectFile> w(m_ObjectFiles);
      boost::default_writer def;
      sample_graph_writer sample;
      boost::write_graphviz(cout, m_Graph.get(),
        boost::make_label_writer(w), def, sample);

      wrapper<ObjectPackage> w2(m_packages);
      boost::write_graphviz(cout, m_Graph2.get(),
        boost::make_label_writer(w2));
   }
   
private:
   filelist_t list;
   SymbolStore m_symbols;
   Graph<ObjectFile> m_Graph;
   Graph<ObjectPackage> m_Graph2;
   std::set<ObjectPackage> packages;
   std::vector<ObjectPackage*> m_packages;
   std::vector<ObjectFile*> m_ObjectFiles;
};
