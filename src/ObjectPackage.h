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

#ifndef OBJECT_PACKAGE_H
#define OBJECT_PACKAGE_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include "Entity.h"
#include "Callback.h"

class ObjectFile;

class ObjectPackage : public Entity
{
public:
   ObjectPackage(Callback&, const std::string& name);
//   virtual void AddImport(Package* p);
//   virtual void AddExport(Package* p);

//   void Link();
   void Link(Entity&);

   bool operator < (const ObjectPackage& rhs) const
   {
     return Name() < rhs.Name();
   }

public:
   std::set<ObjectFile*> m_Requires;
   std::set<ObjectFile*> m_Provides;
private:
   Callback& m_Callback;
};

#endif
