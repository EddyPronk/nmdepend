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

#include "Package.h"

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>

#include <algorithm>
#include <iostream>

namespace fs = boost::filesystem;


#if 0
const char* include = "#[[:space:]]*include[[:space:]]*" // # include
                      "(?:\"|<)"                         // " or <
		      "([[:word:]]+(?:\\.[[:word:]]+)?)"  // filename
		      "(?:\"|>)";                        // " or >
#else
const char* include = "#[[:space:]]*include[[:space:]]*" // # include
                      "\""                               // "
		      "([[:word:]]+(?:\\.[[:word:]]+)?)"  // filename
		      "\"";                              // "
#endif

void recurse_directory(const fs::path& p)
{
  try
  {
    boost::regex expr(include);

    std::string extension(fs::extension(p));

    if(  extension == ".h"
      || extension == ".c"
      || extension == ".hpp"
      || extension == ".cpp"
      )
    {
      fs::ifstream file(p);
      std::string line;
      while(file)
      {
        std::getline(file, line);

        std::string::const_iterator begin(line.begin());
        std::string::const_iterator end(line.end());
        boost::match_flag_type flags(boost::match_default);
        boost::match_results<std::string::const_iterator> what;

        while(boost::regex_search(begin, end , what, expr, flags))
        {
	  std::string filename1(p.leaf());
	  std::string filename2(what[1].first, what[1].second);
	  filename2 = fs::path(filename2).leaf();

	  std::cout << '"' << filename1 << "\" -> \""
                           << filename2 << "\";" << std::endl;

	  begin = what[0].second;
	  flags |= boost::match_prev_avail;
	  flags |= boost::match_not_bob;
        }
      }
    }
    else
    {
      if(fs::is_directory(p))
      {
        std::string cluster;
        try
        {
          cluster = fs::basename(fs::complete(p).leaf());
        }
        catch(std::exception& e)
        {
          std::clog << "exception in: " << __FILE__ << " at: " << __LINE__
                    << ": " << e.what() << std::endl;
        }

        std::cout << "subgraph \"cluster_" << cluster
                  << "\"{\nlabel = \"" << cluster << "\";" << std::endl;

        fs::directory_iterator end;
        for(fs::directory_iterator it(p); it != end; ++it)
        {
          std::string extension(fs::extension(*it));

          if(  extension == ".h"
            || extension == ".c"
            || extension == ".hpp"
            || extension == ".cpp"
            )
          {
            std::cout << '"' << it->leaf() << "\";" << std::endl;
          }
        }
        std::cout << '}' << std::endl;
        
        for(fs::directory_iterator it(p); it != end; ++it)
        {
          recurse_directory(*it);
        }

      }
    }
  }
  catch(std::exception& e)
  {
    std::clog << "exception in: " << __FILE__ << " at: " << __LINE__
              << ": " << e.what() << std::endl;
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if(argc > 2 && argv[1] == std::string("-i"))
    {
      boost::regex expr(include);

      std::string line;
      std::cout << "digraph G{\nnodesep=1;\n"
                   "ranksep=2;\n"
		   "node [shape = box];" << std::endl;

      for(int i = 2; i < argc; ++i)
      {
        recurse_directory(argv[i]);
      }
      std::cout << "}" << std::endl;
    }
    else if(argc > 2 && argv[1] == std::string("-o"))
    {
      std::cout << "digraph G {" << std::endl
                << "nodesep = 0.5;" << std::endl
                << "ranksep = 1;" << std::endl
                << "node [shape = box];" << std::endl;

      std::clog << "digraph G{\n"
		   "node [shape = box];" << std::endl;

      if(argc > 1)
      {
        if(fs::is_directory(argv[2]))
        {
          Package p(argv[2]);

	  p.PackDepend(p);
        }
        else
        {
          std::clog << "error: enter the top directory of the project"
                    << std::endl;
        }
      }

      std::cout << "}" << std::endl;
      std::clog << "}" << std::endl;
    }
    else
    {
      std::cout << "usage:\ndepend -i projectdir or -o projectdir\n"
                << std::endl;
    }
  }
  catch(std::exception& e)
  {
    std::clog << "exception: " << e.what() << std::endl;
  }
  return 0;
}
