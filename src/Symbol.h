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

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <set>
#include <map>

#include "Package.h"

class ObjectFile;
class Symbol;

typedef Symbol* SymbolPtr;

class Symbol
{
public:
   typedef std::string Name_t;
   typedef std::map<Name_t, Symbol> SymbolIndex_t;

   Symbol(ObjectFile* o, const std::string& name);
   Symbol(const std::string& name);
   const std::string& Name() const;
   bool operator < (const Symbol& rhs) const;
   bool foundOwner() const;
   void setOwner(ObjectFile* o);
   ObjectFile* Owner() const;   
private:
   ObjectFile* m_Owner;
   std::string m_Name;
};

#endif
