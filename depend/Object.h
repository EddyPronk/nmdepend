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

#ifndef OBJECT_H
#define OBJECT_H

#include "Symbol.h"
 
#include <boost/filesystem/path.hpp>

#include <map>
 
class Package;
 
class Object
{
  struct ImportExport
  {
    ImportExport(): refcount(0) {}
    unsigned int refcount;
    std::set<Symbol> Import;
    std::set<Symbol> Export;
  };
public:
  typedef std::string Name_t;
  typedef std::map<Object::Name_t, ImportExport > ObjectRegistry;
private:
  static ObjectRegistry s_ObjectRegistry;
  ObjectRegistry::iterator m_Object;
public:
  Object(const boost::filesystem::path& p);
  Object(const Object& o);
  ~Object();
  bool operator < (const Object& rhs) const;
  bool operator == (const Object& rhs) const;
  bool operator != (const Object& rhs) const;
  void Import(const Symbol& s);
  void Export(const Symbol& s);
  const std::string& Name() const;
  bool Depend(const Symbol& s) const;
  bool Depend(const Object& o) const;
  bool Depend(const Package& p) const;
};


 
 #endif
 
