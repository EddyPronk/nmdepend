#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <vector>
#include <set>
#include <map>

// Package does not implement Provides/Requires containers.

class Package
{
public:
   typedef std::string Name_t;
   typedef std::set<Package*> Index_t;
   typedef std::vector<Package*> SubPackageList_t;
   typedef std::map<Name_t, Package*> PackageRegistry_t;


   Package(const Name_t& name);

   void SetParent(Package& parent);
   //Package& Parent();
   void Imports();
   void Imports(SubPackageList_t& list);
   Name_t& Name();

   // todo Rename/remove this
   Name_t Name2();

   // Two types of relationships are being stored.
   // Import/Export represent dependencies between packages on same level.
   // Requires/Provides is the same as undefined/defined for objectfiles.
   // These names are choosen to abstract from objectfiles.
   //
   // Import/Export has different semantics as in UML.
   //
   //   Provides is a 'has' relationship
   //   Requires is a list o

   virtual void AddRequires(Package* p);
   virtual void AddProvides(Package* p);
   virtual void AddImport(Package* p);
   virtual void AddExport(Package* p);

private:

   // For storing inter package dependencies
   std::set<Package*> m_Imports;
   std::set<Package*> m_Exports;

   Name_t m_Name;

protected:
   Package* m_Parent;
};

#endif
