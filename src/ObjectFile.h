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
#include "Entity.h"
#include "Symbol.h"
#include "SymbolStore.h"
#include "ObjectPackage.h"

class bfd;
class Symbol;

class ObjectFile : public Entity
{
public:
  ~ObjectFile() {}
//  typedef std::set<SymbolPtr> SymIndex_t;

  ObjectFile(DependencyAddedEvent&,
			 SymbolAdded&,
			 const std::string& name,
			 SymbolStore& store);

  void Read(const boost::filesystem::path& objectfile);
  void Read(bfd* file);
  void AddImportSymbol(const std::string& name);
  void AddExportSymbol(const std::string& name);

  // Link objects like a real linker does.
  void Link();

  bool Depend(const Entity& o) const;

  void intersection(const Entity& rsh, SymIndex_t& i) const;

private:
   DependencyAddedEvent& on_dependency_added;
   SymbolAdded& on_symbol_added;

   // List of imported symbols
   SymIndex_t m_SymImports;

   // List of exported symbols.
   SymIndex_t m_SymExports;

   // For looking up in which objectfile a symbol is defined.
   SymbolStore& m_SymbolStore;
};

#endif
