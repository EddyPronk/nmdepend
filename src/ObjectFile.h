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

#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include <map>
#include <set>
#include <string>
#include <iostream>

#include "boost/filesystem/path.hpp"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/fstream.hpp>

#include "Entity.h"
#include "Package.h"
#include "Symbol.h"

class bfd;
class Symbol;

class ObjectFile : public Package
{
public:
  typedef std::set<Symbol*> SymIndex_t;

  ObjectFile(const Package::Name_t& name, Symbol::SymbolIndex_t& symbolIndex);

  void Read(const boost::filesystem::path& objectfile);
  void Read(bfd* file);
  void AddImportSymbol(const std::string& name);
  void AddExportSymbol(const std::string& name);

  // Link objects like a real linker does.
  void Link();

  // Experimental operation to play with intersection
  void Boo(ObjectFile& rsh, SymIndex_t& i);

private:
   ObjectFile();

   // List of imported symbols
   SymIndex_t m_SymImports;

   // List of exported symbols.
   SymIndex_t m_SymExports;

   // For looking up in which objectfile a symbol is defined.
   Symbol::SymbolIndex_t& m_SymbolIndex;
};

#endif
