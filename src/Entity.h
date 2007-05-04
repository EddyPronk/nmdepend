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

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "Symbol.h"

class Entity
{
public:
  typedef std::set<SymbolPtr> SymIndex_t;

  Entity(const std::string& name);
  virtual ~Entity() {}
  void SetParent(Entity& p);
  Entity* Parent();
  const std::string& Name() const;
  virtual void Link() {}
  virtual void Link(Entity&) {}
  virtual bool Depend(const Entity&) const { return false; }
//  virtual void intersection(const Entity&, SymIndex_t& i) const {}
public:
	void print(std::ostream& os) const
	{
		os << m_Name;
	}
private:
  std::string m_Name;
  Entity* m_Parent;
};

std::ostream& operator<<(std::ostream& out, const Entity& e);	

#endif

