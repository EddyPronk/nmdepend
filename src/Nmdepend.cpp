#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <cc++/common.h>

#include "ObjectFile.h"

using namespace std;
using namespace ost;

namespace fs = boost::filesystem;

CommandOptionNoArg	option_recurse(
	"recurse", "r", "search for objects recursively"
);

CommandOptionArg	option_style(
	"style", "s", "tree style 'auto' (default), 'vstudio' ", true
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

typedef std::vector<fs::path> filelist_t;

// todo : match wildcards with filter_iterator and regex (*.o) (*.a) (*.lib)
void find_file( const fs::path& dir_path, filelist_t& list)
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
         find_file(*itr, list);
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

int main( int argc, char ** argv )
{
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
  	
   if ( helparg.numSet )
   {
     	cerr << args->printUsage();
     	::exit(0);
	}

   args->performTask();

   filelist_t list;

   // print all the other options.
	for ( int i = 0; i < restoargs.numValue; i ++ ) {
      fs::path root(restoargs.values[i]);
      find_file(root, list);//creates list of *.o files
	}

   Symbol::SymbolIndex_t symbolIndex;

   //why use vectors of pointers and not vectors of objects?
   //this uses two indirections instead of one;
   //todo change to object container instead of pointer to object container
   typedef std::vector<ObjectFile*> ObjectList_t;
   typedef std::vector<Package*> PackageList_t;

   ObjectList_t objectList;
   PackageList_t packageList;
   Package::PackageRegistry_t packages;

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

      ObjectFile* o = new ObjectFile(name,symbolIndex); //memory leak

      Package* pack = packages[packagename];
      if (pack == 0)
      {
         //std::cout << "Adding package " << packagename << std::endl;
         pack = new Package(packagename); //memory leak
         packages[packagename] = pack;
      }

      o->Read(*pos);
      o->SetParent(*pack);
      objectList.push_back(o);
   }

   for (ObjectList_t::iterator pos = objectList.begin();
        pos != objectList.end();
        ++pos)
   {
      (*pos)->Link();
   }

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
