#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <algorithm>
#include <set>
#include <map>
#include <iostream>
//#include "Dependency.h"

#include "ObjectFile.h"
#include "Package.h"

//#include "bfd.h"
//#include "../binutils/binutils-20040312-1-modified/include/libiberty.h"

//#define typename type_name
//#include "../binutils/binutils-20040312-1-modified/include/demangle.h"


//typedef std::set<Dependency*> DependencyList;

#if 0
void f(Dependency* x)
{
   std::cout << x->m_From->Name() << " -> " << x->m_To->Name() << std::endl;

}
#endif


int main(void)
{
//   std::cout << sizeof(Dependency) << std::endl;
//   std::cout << sizeof(PackageDependency) << std::endl;

   // Pass 1 : Read all objectfiles and store the import and export symbols

   Symbol::SymbolIndex_t symbolIndex;

   Package aa("aa"); // superpackage of a
   ObjectFile a("a.obj", symbolIndex);
   a.SetParent(aa);

   std::cout << "step 1" << std::endl;

//   a.AddImportSymbol("a");
   a.AddImportSymbol("b");
   a.AddImportSymbol("c");
   a.AddImportSymbol("c");
   a.AddImportSymbol("d");

   std::cout << "step 2" << std::endl;

   Package bb("bb"); // superpackage of a
   ObjectFile b("b.obj", symbolIndex);
   b.SetParent(bb);
   b.AddExportSymbol("a");
   b.AddExportSymbol("b");
   b.AddExportSymbol("c");
   //b.AddExportSymbol("d");
   a.Link();

   ObjectFile::SymIndex_t inter;

   std::cout << "test intersection" << std::endl;

   b.Boo(a, inter);
   for(ObjectFile::SymIndex_t::iterator i = inter.begin(); i != inter.end(); ++i)
   {
      std::cout << (*i)->m_Name << std::endl;
   }

   a.Imports();
   aa.Imports();

//   Dependency* d = new Dependency(a,b);
//   Dependency dd = *d;

//   Dependency::Index_t dependencyIndex;
//   dependencyIndex.push_back(d);

   // welke symbolen horen bij dependency d?
   //d->print();
   //dependencyIndex.insert(Dependency(a,b));

//   std::for_each(dependencyIndex.begin(), dependencyIndex.end(), f);



   // Package x imports symbol a
//   symbolIndex["a"]->Add(x);

//   std::cout << symbolIndex["a"]->m_Owner->Name() << std::endl;

//   ObjectFile b;

return 0;
}
