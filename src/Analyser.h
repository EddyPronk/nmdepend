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
  }
};

struct wrapper
{
  wrapper(vector<ObjectFile*>& v) : m_v(v) {}
  const string& operator[](int i) const
  {
    return m_v[i]->Name();
  }
  const vector<ObjectFile*>& m_v;
};

class Analyser
{
public:
   Analyser() : m_dummypackage(m_Graph2, "dummy")
   {
   }
   
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
         o->Read(*pos);
         o->SetParent(m_dummypackage);
         m_ObjectFiles.push_back(o);
      }
   }
   void Link()
   {
      m_Graph.init(m_ObjectFiles);
      for (std::vector<ObjectFile*>::iterator pos = m_ObjectFiles.begin();
      pos != m_ObjectFiles.end();
      ++pos)
      {
         std::cout << "linking obj " << (*pos)->Name() << std::endl;
         (*pos)->Link();
      }
   }
   
private:
   ObjectPackage m_dummypackage;
   filelist_t list;
   SymbolStore m_symbols;
   Graph m_Graph;
   CallBackDummy<ObjectPackage> m_Graph2;
   
   std::vector<ObjectFile*> m_ObjectFiles;
};
