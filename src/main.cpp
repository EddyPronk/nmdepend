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

#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <fstream>
#include <iterator>
#include "Analyser.h"

using namespace std;

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
  copy(v.begin(), v.end(), ostream_iterator<T>(cout, " ")); 
  return os;
}

const char* version()
{
  return "This is nmdepend\n"
  "\n"
  "Copyright 2004-2007, Eddy Pronk, Rutger van Beusekom\n"
  "\n"
  "This program is free software; you may redistribute it under the terms of\n"
  "the GNU General Public License.  This program has absolutely no warranty.\n"
  "\n";
}

struct PrinterFactory
{
	SymbolAdded no_printing;
	PrintSymbols print_symbols_;
	PrintSymbolsAndDemangle print_symbols_and_demangle_;
	SymbolAdded& get(po::variables_map& options)
	{
		if(!options.count("print"))
		   return no_printing;

		if(options.count("demangle"))
			return print_symbols_and_demangle_;
		else
			return print_symbols_;
	}
};

int main(int ac, char* av[])
{
	try
	{
		int option_level;

		// Declare a group of options that will be 
		// allowed only on command line
		po::options_description generic("Generic options");
		generic.add_options()
			("version,v", "print version string")
			("help,h", "produce help message")
			("print,p", "print symbols")
			("demangle,d", "demangle symbols");

		// Declare a group of options that will be 
		// allowed both on command line and in
		// config file
		po::options_description config("Configuration");
		config.add_options()
			("package-level,n", po::value<int>(&option_level)->default_value(1), 
			 "extract name from path at level <n>")
			("library,l", po::value< vector<string> >()->composing(), "library")
			("package,p", po::value< vector<string> >()->composing(),
			 "threat library as directory");

		// Hidden options, will be allowed both on command line and
		// in config file, but will not be shown to the user.
		po::options_description hidden("Hidden options");
		hidden.add_options()
			("input-file", po::value< vector<string> >(), "input file");


		po::options_description cmdline_options;
		cmdline_options.add(generic).add(config).add(hidden);

		po::options_description config_file_options;
		config_file_options.add(config).add(hidden);

		po::options_description visible("Allowed options");
		visible.add(generic).add(config);

		po::positional_options_description p;
		p.add("input-file", -1);

		po::variables_map options;
		store(po::command_line_parser(ac, av).
			  options(cmdline_options).positional(p).run(), options);

		ifstream ifs("multiple_sources.cfg");
		store(parse_config_file(ifs, config_file_options), options);
		notify(options);

		if (options.count("help")) {
			cout << visible << "\n";
			return 0;
		}

		if (options.count("version"))
		{
			cout << version();
			return 0;
		}

		if (options.count("library"))
		{
			cout << "Libraries are: " 
				 << options["library"].as< vector<string> >() << "\n";
		}

		if (options.count("package"))
		{
			cout << "Packages are: " 
				 << options["package"].as< vector<string> >() << "\n";
		}

		if (options.count("input-file"))
		{
			cout << "Input files are: " 
				 << options["input-file"].as< vector<string> >() << "\n";
		}

// cout << "Optimization level is " << option_level << "\n";

		//SymbolAdded on_symbol_added;
		PrinterFactory printer_factory_;
		Analyser app(option_level, printer_factory_.get(options));
		if(options.count("input-file"))
		{
			app.Add(options["input-file"].as< vector<string> >());
		}

//  app.Add(options["library"].as< vector<string> >());

		if(options.count("package"))
		{
			app.AddPackages(options["package"].as< vector<string> >());
		}
		app.ReadObjects();
		app.Link();

		std::ofstream objectStream;
		objectStream.open("object.dot");
		app.WriteObjectGraph(objectStream);

		std::ofstream packageStream;
		packageStream.open("package.dot");
		app.WritePackageGraph(packageStream);
	}
	catch(exception& e)
	{
		cout << e.what() << "\n";
		return 1;
	}    
	return 0;
}

