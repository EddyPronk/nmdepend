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

#include <cassert>
#include <iostream>
#include "ObjectPackage.h"

ObjectPackage::ObjectPackage(Callback& callback, const std::string& name)
 : Entity(name)
 , m_Callback(callback)
{
}

#if 0
void ObjectPackage::AddRequires(Package* p)
{
   m_Requires.insert(p);
   std::cout << Name() << " requires " << p->Name() << std::endl;
}

void ObjectPackage::AddProvides(Package* p)
{
   m_Provides.insert(p);
   std::cout << Name() << " provides " << p->Name() << std::endl;
}
#endif

//void ObjectPackage::AddImport(Package* p)
//{
   //std::cout << "ObjectPackage::AddImport" << std::endl;
   //Package::AddImport(p);
//}

//void ObjectPackage::AddExport(Package* p)
//{
   //   std::cout << "ObjectPackage::AddExport" << std::endl;
   //Package::AddExport(p);
//}

//void ObjectPackage::Link()
//{
//!   for(std::set<ObjectFile*>::iterator pos = m_Provides.begin(); pos != m_Provides.end(); ++pos)
  // {
      //std::cout << "ObjectPackage::Link " << (*pos)->Name() << std::endl;
      //(*pos)->Link();
   //}
//}

#if 0
void ObjectPackage::Provides(SubPackageList_t& list)
{
   list.clear();
   for(Index_t::iterator pos = m_Provides.begin()
      ; pos != m_Provides.end(); ++pos)
   {
      list.push_back(*pos);
      std::cerr << Name() << " ObjectPackage::provides " << (*pos)->Name() << std::endl;
   }
}
#endif

void ObjectPackage::Link(Entity& rsh)
{
  if (this != &rsh)
  {
    m_Callback(*this, rsh);
    if(Parent())
    {
      Parent()->Link(*rsh.Parent());
    }
  }
}

