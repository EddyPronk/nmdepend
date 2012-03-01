// Copyright (c) 2005 E.J. Pronk and R.E.W. van Beusekom
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
#include "boost/filesystem/convenience.hpp"
#include "Analyser.h"

namespace fs = boost::filesystem;
using namespace std;

Analyser::Analyser(int packageLevel, SymbolAdded& on_symbol_added)
 : m_Bfd(m_Factory)
 , m_Factory(m_ObjectGraph, m_PackageGraph, on_symbol_added)
 , m_packageLevel(packageLevel)
{
}


void Analyser::Add(const std::vector<string>& files)
{
  for(vector<string>::const_iterator pos = files.begin() ;
      pos != files.end(); ++pos)
  {
    fs::path path(*pos);
    find_file(path);
  }
}

void Analyser::AddPackages(const std::vector<string>& packages)
{
  for(vector<string>::const_iterator pos = packages.begin() ;
      pos != packages.end(); ++pos)
  {
    cout << "add " << *pos << endl;

    fs::path path(*pos);
    m_Bfd.ReadPackage(*pos, m_packageLevel);
  }
}
  
void Analyser::find_file( const fs::path& dir_path)
{
  if ( !fs::exists( dir_path ) )
  {
    std::cout << "doesn't exist " << dir_path.string() << std::endl;
  }

  if ( fs::is_directory( dir_path ))
  {

  fs::directory_iterator end_itr;
  for(fs::directory_iterator itr( dir_path );
      itr != end_itr;
      ++itr )
      {
         if ( fs::is_directory( *itr ) )
         {
            find_file(*itr);
         }
         else
         {
         	
           //if (itr->leaf().rfind(".o") != std::string::npos)
	   if (fs::extension(*itr) == ".o")
	      {
		list.push_back(*itr);
	      }
         }
      }
  }
  else
  {
    list.push_back(dir_path);
  }
}

   void Analyser::ReadObjects()
   {
      for(filelist_t::iterator pos = list.begin(); pos != list.end(); ++pos)
      {
        m_Bfd.Read(*pos, m_packageLevel);
      }
      
      for(std::set<ObjectPackage>::iterator i = m_Factory.m_PackageSet.begin(); i != m_Factory.m_PackageSet.end(); ++i)
      {
        m_Packages.push_back(const_cast<ObjectPackage*>(&(*i)));
      }
   }

void Analyser::Link()
{
  m_ObjectGraph.init(m_Factory.m_ObjectFiles);
  m_PackageGraph.init(m_Packages);
  for (std::vector<Entity*>::iterator pos = m_Factory.m_ObjectFiles.begin();
       pos != m_Factory.m_ObjectFiles.end();
       ++pos)
    {
      std::cout << "linking obj " << (*pos)->Name() << std::endl;
         (*pos)->Link();
    }
  m_Factory.m_symbols.Statistics();
}
   
void Analyser::WriteObjectGraph(std::ostream& out)
{
  my_label_writer lw(m_Factory.m_ObjectFiles);
  boost::write_graphviz(out, m_ObjectGraph.get(), lw);
}
   
void Analyser::WritePackageGraph(std::ostream& out)
{
  my_label_writer lw(m_Packages);
  boost::write_graphviz(out, m_PackageGraph.get(), lw);
}
