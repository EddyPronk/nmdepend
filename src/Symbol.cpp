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

// Macro to deal with const char *typename in demangle.h.
// Don't use the keyword typename in this file and expect
// it to work.
#define typename type_name
#include "demangle.h"
#undef typename



Symbol::Symbol(ObjectFile* o, const std::string& name) : m_Owner(o), m_Name(name)
{
}

std::string Symbol::Demangled()
{
   std::string demangled;

   demangling_styles style = auto_demangling;
   if (m_Name[0] == '_')
   {
      style = gnu_v3_demangling;
   }

   if (m_Name[0] == '?')
   {
   //   style = msvc_demangling;
   }

#ifdef _MSC_VER
   style = msvc_demangling; // TODO remove
#endif
   cplus_demangle_set_style (style);
   
   char* res = cplus_demangle (m_Name.c_str(), DMGL_ANSI | DMGL_PARAMS);
 
   if (res != 0)
   {
      demangled = res;
   }
   
   free(res);
   
   return demangled;
}

