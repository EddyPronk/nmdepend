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

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <cc++/common.h>

#include "ObjectFile.h"
#include "ObjectPackage.h"
#include "SymbolStore.h"

using namespace std;
using namespace ost;

namespace fs = boost::filesystem;

CommandOptionNoArg	option_recurse(
	"recurse", "r", "search for objects recursively"
);

CommandOptionArg	option_style(
	"style", "s", "tree style 'auto' (default), 'vstudio' ", true
);

CommandOptionArg	option_level(
	"level", "l", "Package name at level <n>"
);

CommandOptionNoArg	helparg(
	"help", "h", "Display this information"
);

CommandOptionNoArg	option_version(
	"version", "v", "Display this program's version number"
);

CommandOptionCollect	restoargs(
	0, 0, "Collect all the parameters", true
);

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
      for(filelist_t::iterator pos = list.begin(); pos != list.end(); ++pos)
      {
         //todo define packages as collection of object files
         //using config setting
         fs::path::iterator p = pos->end();
         --p; //file
         std::string name = *p;
         --p; //directory containing file -> Release or Debug using msvc
         --p; // 1 level higher for visual studio
         std::string packagename = *p;

         ObjectFile* o = new ObjectFile(name,m_symbols); //memory leak

         Package* pack = packages[packagename];
         if (pack == 0)
         {
            //std::cout << "Adding package " << packagename << std::endl;
            pack = new ObjectPackage(packagename); //memory leak
            packages[packagename] = pack;
         }

         o->Read(*pos);
         o->SetParent(*pack);
         objectList.push_back(o);
      }
   }
   
   void Link()
   {
      for (ObjectList_t::iterator pos = objectList.begin();
      pos != objectList.end();
      ++pos)
      {
         std::cout << "linking obj " << (*pos)->Name() << std::endl;
         (*pos)->Link();
      }
   }
   
   void WriteObjectGraph()
   {
      std::ofstream dotfile;
      dotfile.open("object.dot");
      dotfile << "digraph G {" << std::endl;
      dotfile << "node [shape=box];"  << std::endl;

      for (ObjectList_t::iterator pos = objectList.begin();
      pos != objectList.end();
      ++pos)
      {
         Package::SubPackageList_t list;
         (*pos)->Imports(list);
         for (Package::SubPackageList_t::iterator package = list.begin();
         package != list.end();
         ++package)
         {
            dotfile << (*pos)->Name2() << " -> "
               << (*package)->Name2() << ";" << std::endl;
         }
      }

      dotfile << "}" << std::endl;
      dotfile.close();
   }

   void WritePackageGraph()
   {
      std::ofstream dotfile;
      dotfile.open("package.dot");
      dotfile << "digraph G {" << std::endl;
      dotfile << "node [shape=box];"  << std::endl;

      for (Package::PackageRegistry_t::iterator pos = packages.begin();
      pos != packages.end();
      ++pos)
      {
         Package::SubPackageList_t list;
         pos->second->Imports(list);
         for (Package::SubPackageList_t::iterator package = list.begin();
         package != list.end();
         ++package)
         {
            dotfile << pos->first << " -> " << (*package)->Name2()
               << ";" << std::endl;
            //std::cout << "Super " << pos->first << " -> "
            //          << (*package)->Name2() << ";" << std::endl;
            //(*package)->Imports();
         }
      }

      dotfile << "}" << std::endl;
      dotfile.close();
   }

   void WritePackageGraph2()
   {
      std::ofstream dotfile;
      dotfile.open("package2.dot");
      dotfile << "digraph G {" << std::endl;
      dotfile << "node [shape=box];"  << std::endl;

      for (Package::PackageRegistry_t::iterator pos = packages.begin();
      pos != packages.end();
      ++pos)
      {
         dotfile << "subgraph " << pos->first << " {" << std::endl;

         Package::SubPackageList_t list;
         pos->second->Provides(list);
         for (Package::SubPackageList_t::iterator package = list.begin();
         package != list.end();
         ++package)
         {
            dotfile << (*package)->Name2() << ";" << std::endl;
         }

         dotfile << "color=blue;" << std::endl;
         dotfile << "}" << std::endl;

         for (Package::SubPackageList_t::iterator package = list.begin();
         package != list.end();
         ++package)
         {
            Package::SubPackageList_t list2;
            pos->second->Imports(list2);
            for (Package::SubPackageList_t::iterator package2 = list2.begin();
            package2 != list2.end();
            ++package2)
            {
               dotfile << (*package)->Name2() << " -> " << (*package2)->Name2()
                  << ";" << std::endl;
            }
         }
      }

      dotfile << "}" << std::endl;
      dotfile.close();
   }

private:
   filelist_t list;
   SymbolStore m_symbols;
   
   //why use vectors of pointers and not vectors of objects?
   //this uses two indirections instead of one;
   //todo change to object container instead of pointer to object container
   typedef std::vector<ObjectFile*> ObjectList_t;
   typedef std::vector<Package*> PackageList_t;
   
   ObjectList_t objectList;
   PackageList_t packageList;
   Package::PackageRegistry_t packages;
};

int main( int argc, char ** argv )
{
   Analyser it;

   try
   {
	CommandOptionParse * args = makeCommandOptionParse(
	   argc, argv,
	   "\n"
      "Usage: nmdepend [option(s)] [file(s)]\n"
      "The options are:"
   );

  	if ( option_version.numSet )
   {
     	cerr << "nmdepend version 0.1.0 (" __DATE__ ")" << endl;
     	::exit(0);
	}

   if (option_level.hasValue())
   {
      int level = atoi(option_level.values[0]);
      cerr << "level " << level << endl;
   }
  	
   if ( helparg.numSet )
   {
     	cerr << args->printUsage();
     	::exit(0);
	}

   // No clue what it does.
   args->performTask();

   // print all the other options.
	for ( int i = 0; i < restoargs.numValue; i ++ ) {
      fs::path root(restoargs.values[i]);
      it.find_file(root); //creates list of *.o files
	}

   it.ReadObjects();
   it.Link();
   it.WriteObjectGraph();
   it.WritePackageGraph();
   it.WritePackageGraph2();

   delete args;

   return 0;
 }
  catch(const std::exception& e)
  {
    std::clog << "exception: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::clog << "caught unknown exception, exiting" << std::endl;
  }
  return 1;

}
