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
   const char* p = m_Name.c_str();

   demangling_styles style = auto_demangling;
   if (*p == '_')
   {
      style = gnu_v3_demangling;
      ++p;
   }

   if (*p == '?')
   {
      style = msvc_demangling;
   }

#ifdef _MSC_VER
   style = msvc_demangling; // TODO remove
#endif

   cplus_demangle_set_style (style);
   char* res = cplus_demangle (p, DMGL_ANSI | DMGL_PARAMS);

   if (res != 0)
   {
      demangled = res;
   }

   return demangled;
}

