// Macro to deal with const char *typename in demangle.h.
// Don't use the keyword typename in this file and expect
// it to work.
#define typename type_name
#include "budemang.h"
#undef typename

#include <iostream>
#include <string>

int main(void)
{
   // std::string sym = "__ZN1BC1Ev";
   std::string sym = "??0B@@QAE@XZ";
   const char* p = sym.c_str();
   ++p; // strip underscore - see budemang.c for a more robust approach.
   //cplus_demangle_set_style (gnu_v3_demangling);
   cplus_demangle_set_style (msvc_demangling);
   char* res = cplus_demangle (p, DMGL_ANSI | DMGL_PARAMS);
   if (res)
   {
      std::cout << "[" << res << "]" << std::endl;
   }
   else
   {
      throw; // something
   }

   return 0;
}
