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

#include "Symbol.h"
#include <cstdlib>

// Macro to deal with const char *typename in demangle.h.
// Don't use the keyword typename in this file and expect
// it to work.
#define typename type_name
#include <demangle.h>
#undef typename

std::set<Symbol::Name_t> Symbol::s_SymbolRegistry;

Symbol::Symbol(const Symbol::Name_t& s)
{
  m_Symbol = s_SymbolRegistry.insert(s_SymbolRegistry.begin(), s);
}
bool Symbol::operator < (const Symbol& rhs) const
{
  return *m_Symbol < *rhs.m_Symbol;
}
const Symbol::Name_t& Symbol::GetName_t() const
{
  return *m_Symbol;
}
Symbol::Name_t Symbol::Demangle() const
{
  std::string ret(*m_Symbol);
  char* demangled = cplus_demangle(m_Symbol->c_str(), gnu_v3_demangling);
  ret = demangled;
  free(demangled);
  return ret;
}
