#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "ObjectPackage.h"
#include <iostream>

ObjectPackage::ObjectPackage(const Name_t& name) : Package(name)
{
}

void ObjectPackage::AddRequires(Package* p)
{
   m_Requires.insert(p);
   std::cout << Name() << " requires " << p->Name() << std::endl;
}

void ObjectPackage::AddProvides(Package* p)
{
   m_Provides.insert(p);
   std::cout << Name() << " provides " << p->Name() << std::endl;
}

void ObjectPackage::AddImport(Package* p)
{
   //std::cout << "ObjectPackage::AddImport" << std::endl;
   Package::AddImport(p);
}

void ObjectPackage::AddExport(Package* p)
{
   //   std::cout << "ObjectPackage::AddExport" << std::endl;
      Package::AddExport(p);
}

