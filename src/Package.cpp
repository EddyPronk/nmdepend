#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "Package.h"
#include <iostream>

Package::Package(const Name_t& name) : m_Name(name), m_Parent(0)
{
}

Package::Name_t& Package::Name()
{
   return m_Name;
}

Package::Name_t Package::Name2()
{
   std::string name = Name();

  //todo configure extension
  //in the meantime using rfind(".o") should work for *.obj too
  return name.substr(0, name.rfind(".o"));
}

void Package::SetParent(Package& parent)
{
   m_Parent = &parent;
   parent.AddProvides(this); // todo remove argumement
   //parent.m_Contains.insert(this);
}

void Package::AddRequires(Package* p)
{
}

void Package::AddProvides(Package* p)
{
}

void Package::AddImport(Package* p)
{
   if (p != this)
   {
      std::cout << "Package::AddImport " << p->Name() << " to " << Name() << std::endl;
      m_Imports.insert(p);
   }

   if (m_Parent && p->m_Parent)
   {
      m_Parent->AddImport(p->m_Parent);
   }
}

void Package::AddExport(Package* p)
{
   if (p != this)
   {
      std::cout << "Package::AddExport " << p->Name() << " to " << Name() << std::endl;
      m_Exports.insert(p);
   }

   if (m_Parent && p->m_Parent)
   {
      m_Parent->AddExport(p->m_Parent);
   }
}

void Package::Imports()
{
   for(Index_t::iterator pos = m_Imports.begin()
      ; pos != m_Imports.end(); ++pos)
   {
      std::cout << Name() << " -> " << (*pos)->Name() << std::endl;
   }
}

void Package::Imports(SubPackageList_t& list)
{
   list.clear();
   for(Index_t::iterator pos = m_Imports.begin()
      ; pos != m_Imports.end(); ++pos)
   {
      list.push_back(*pos);
      std::cout << Name() << " -> " << (*pos)->Name() << std::endl;
   }
}

void Package::Link()
{
}
