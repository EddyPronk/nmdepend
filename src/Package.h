#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <vector>
#include <set>
#include <map>

class Package
{
public:
   typedef std::string Name_t;
   typedef std::set<Package*> Index_t;
   typedef std::vector<Package*> SubPackageList_t;
   typedef std::map<Name_t, Package*> PackageRegistry_t;


   Package(const Name_t& name);

   void SetParent(Package& parent);
   void Imports();
   void Imports(SubPackageList_t& list);
   Name_t& Name();
   Name_t Name2();
   void AddImport(Package* p);
   void AddExport(Package* p);

private:
   private:
   Index_t m_Contains;
   Index_t m_Imports;
   Index_t m_Exports;

   Package* m_Parent;
   Name_t m_Name;
};

#endif
