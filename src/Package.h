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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <vector>
#include <set>
#include <map>

// Package does not implement Provides/Requires containers.

class Package
{
public:
   typedef std::string Name_t;
   typedef std::set<Package*> Index_t;
   typedef std::vector<Package*> SubPackageList_t;
   typedef std::map<Name_t, Package*> PackageRegistry_t;


   Package(const Name_t& name);

   void SetParent(Package& parent);
   //Package& Parent();
   void Imports();
   void Imports(SubPackageList_t& list);
   virtual void Provides(SubPackageList_t& list);
   Name_t& Name();

   // todo Rename/remove this
   Name_t Name2();

   // Two types of relationships are being stored.
   // Import/Export represent dependencies between packages on same level.
   // Requires/Provides is the same as undefined/defined for objectfiles.
   // These names are choosen to abstract from objectfiles.
   //
   // Import/Export has different semantics as in UML.
   //
   //   Provides is a 'has' relationship
   //   Requires is a list o

   virtual void AddRequires(Package* p);
   virtual void AddProvides(Package* p);
   virtual void AddImport(Package* p);
   virtual void AddExport(Package* p);
   virtual void Link();

private:

   // For storing inter package dependencies
   std::set<Package*> m_Imports;
   std::set<Package*> m_Exports;

   Name_t m_Name;

protected:
   Package* m_Parent;
};

#endif
