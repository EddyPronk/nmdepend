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

#include "Analyser.h"
//#include "ObjectFile.h"
//#include "ObjectPackage.h"
//#include "SymbolStore.h"

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
      cerr << "nmdepend version (" __DATE__ ")" << endl;
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

   Analyser app;

   // print all the other options.
	 for ( int i = 0; i < restoargs.numValue; i ++ ) {
      fs::path root(restoargs.values[i]);
      app.find_file(root); //creates list of *.o files
	 }

   app.ReadObjects();
   app.Link();

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
