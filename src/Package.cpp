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

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "Package.h"
#include <iostream>

Package::Package(Callback<Package>& callback, const std::string& name)
 : Entity(name)
 , m_Parent(0)
 , m_Callback(callback)
{
}

std::string Package::Name2()
{
   std::string name = Name();

  //todo configure extension
  //in the meantime using rfind(".o") should work for *.obj too
  return name.substr(0, name.rfind(".o"));
}

/*void Package::SetParent(Package& parent)
{
   m_Parent = &parent;
}*/

void Package::AddImport(Package* p)
{
   if (p != this)
   {
      std::cout << "Package::AddImport " << p->Name() << " to " << Name() << std::endl;
      m_Imports.insert(p);
   }

   if (m_Parent && p->m_Parent)
   {
      m_Parent->AddImport(p->m_Parent);
   }
}

void Package::AddExport(Package* p)
{
   if (p != this)
   {
      std::cout << "Package::AddExport " << p->Name() << " to " << Name() << std::endl;
      m_Exports.insert(p);
   }

   if (m_Parent && p->m_Parent)
   {
      m_Parent->AddExport(p->m_Parent);
   }
}

void Package::Imports(SubPackageList_t& list)
{
   list.clear();
   for(Index_t::iterator pos = m_Imports.begin()
      ; pos != m_Imports.end(); ++pos)
   {
      list.push_back(*pos);
      std::cout << Name() << " -> " << (*pos)->Name() << std::endl;
   }
}

/*void Package::Provides(SubPackageList_t& list)
{
   std::cout << "Provides" << std::endl;
}*/

void Package::Link()
{
}

bool Package::Depend(const Package& package) const
{
  return true; 
}

void Package::Link(Package* rsh)
{
  m_Callback(*this, *rsh);
}

