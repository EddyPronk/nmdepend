#include "Archive.h"

#include "bfd.h"
#include <iostream>

int main( int argc, char* argv[] )
{
   Symbol::SymbolIndex_t symbolIndex;

   Archive a("aaa");
   a.Read("/lib/libbfd.a", symbolIndex);
   a.Link();
   return 0;
}
