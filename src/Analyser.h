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

   Analyser(int packageLevel) : m_packageLevel(packageLevel)
   {
   }
   
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
      std::cout << "package level << " << m_packageLevel << std::endl;
       // temporary
      for(filelist_t::iterator pos = list.begin(); pos != list.end(); ++pos)
      {
         //todo define m_PackageSet as collection of object files
         //using config setting
         fs::path::iterator p = pos->end();
         --p; //file
         std::string name = *p;
         --p; //directory containing file -> Release or Debug using msvc
         if(m_packageLevel == 2)
         {
           --p;
         }
         std::string packagename = *p;
         
         cout << pos->string() << endl;
         ObjectFile* o = new ObjectFile(m_ObjectGraph, name, m_symbols);

         std::pair<std::set<ObjectPackage>::iterator,bool> status =
              m_PackageSet.insert(ObjectPackage(m_PackageGraph, packagename));

         ObjectPackage* op = const_cast<ObjectPackage*>(&(*(status.first)));
         o->Read(*pos);
         o->SetParent(*op);
         m_ObjectFiles.push_back(o);
      }
      
      for(std::set<ObjectPackage>::iterator i = m_PackageSet.begin(); i != m_PackageSet.end(); ++i)
      {
        m_Packages.push_back(const_cast<ObjectPackage*>(&(*i)));
      }
   }
   void Link()
   {
      m_ObjectGraph.init(m_ObjectFiles);
      m_PackageGraph.init(m_Packages);
      for (std::vector<ObjectFile*>::iterator pos = m_ObjectFiles.begin();
      pos != m_ObjectFiles.end();
      ++pos)
      {
         std::cout << "linking obj " << (*pos)->Name() << std::endl;
         (*pos)->Link();
      }
      m_symbols.Statistics();
   }
   
   void WriteObjectGraph(std::ostream& out)
   {
      wrapper<ObjectFile> w(m_ObjectFiles);
      boost::default_writer def;
      sample_graph_writer sample;
      boost::write_graphviz(out, m_ObjectGraph.get(), boost::make_label_writer(w), def, sample);
   }
   
   void WritePackageGraph(std::ostream& out)
   {
      wrapper<ObjectPackage> w2(m_Packages);
      boost::write_graphviz(out, m_PackageGraph.get(),
        boost::make_label_writer(w2));
   }
   
private:
   filelist_t list;
   SymbolStore m_symbols;
   Graph<ObjectFile> m_ObjectGraph;
   Graph<ObjectPackage> m_PackageGraph;
   std::set<ObjectPackage> m_PackageSet;
   std::vector<ObjectPackage*> m_Packages;
   std::vector<ObjectFile*> m_ObjectFiles;
   int m_packageLevel;
};
