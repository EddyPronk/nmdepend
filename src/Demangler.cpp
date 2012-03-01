// Macro to deal with const char *typename in demangle.h.
// Don't use the keyword typename in this file and expect
// it to work.

#include <string>
#include <cstdlib>
#define typename type_name
#include "demangle.h"
#undef typename

std::string Demangled(const std::string& name)
{
   std::string demangled;

   demangling_styles style = auto_demangling;
   if (name[0] == '_')
   {
      style = gnu_v3_demangling;
   }

   if (name[0] == '?')
   {
   //   style = msvc_demangling;
   }

#ifdef _MSC_VER
   style = msvc_demangling; // TODO remove
#endif
   cplus_demangle_set_style (style);
   
   char* res = cplus_demangle (name.c_str(), DMGL_ANSI | DMGL_PARAMS);
 
   if (res != 0)
   {
      demangled = res;
   }
   
   free(res);
   
   return demangled;
}

#if 0
        std::string demangled = r->Demangled();
        if (demangled.find("scalar deleting destructor") == std::string::npos)
        {
//          std::cout << "symbol " << s << "::" << demangled << " found in "
//                    << owner->Name() << std::endl;
        }
        else
        {
//          std::cout << "ignored " << s << "::" << demangled << " found in "
//                    << owner->Name() << std::endl;
        }
#endif
