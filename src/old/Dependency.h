#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include <list>
#include <set>
#include "IPackage.h"

class IPackage;

class Dependency
{
public:
   typedef std::list<Dependency*> Index_t;

   Dependency(IPackage& from, IPackage& to);
   
public:
   IPackage* m_From;
   IPackage* m_To;
};

class PackageDependency : public Dependency
{
public:
   PackageDependency();
private:
   std::set<Dependency*> m_core;
};

#endif
