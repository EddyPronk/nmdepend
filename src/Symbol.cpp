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

#include <iostream>
#include "Symbol.h"


//!Symbol::Symbol(ObjectFile* obj, const std::string& name) : m_Owner(obj), m_Name(name)
//{
//}

Symbol::Symbol(const std::string& name) : m_Owner(0), m_Name(name)
{
}

const std::string& Symbol::Name() const
{
  return m_Name;
}

bool Symbol::operator < (const Symbol& rhs) const
{
  return m_Name < rhs.m_Name;
}

 bool Symbol::foundOwner() const
 {
   return m_Owner != 0;
 }
 
 void Symbol::setOwner(ObjectFile* obj)
 {
   m_Owner = obj;
 }
 