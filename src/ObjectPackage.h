#ifndef OBJECT_PACKAGE_H
#define OBJECT_PACKAGE_H

#include <string>
#include <vector>
#include <set>
#include <map>

#include "Package.h"

class ObjectFile;

// Implements Requires/Provides containers.

class ObjectPackage : public Package
{
public:
   ObjectPackage(const std::string& name);
   virtual void AddRequires(Package* p);
   virtual void AddProvides(Package* p);
   virtual void AddImport(Package* p);
   virtual void AddExport(Package* p);


private:
   std::set<Package*> m_Requires;
   std::set<Package*> m_Provides;
};

#endif
