#include "Factory.h"
#include "ObjectFile.h"

Factory::Factory(Callback& object, Callback& package, SymbolAdded& symbol)
 : m_ObjectGraph(object)
 , m_PackageGraph(package)
 , on_symbol_added(symbol)
{
}

ObjectPackage* Factory::CreatePackage(const std::string& name)
{
  std::pair<std::set<ObjectPackage>::iterator,bool> status =
    m_PackageSet.insert(ObjectPackage(m_PackageGraph, name));

  return  const_cast<ObjectPackage*>(&(*(status.first)));
}

ObjectFile* Factory::CreateObject(const std::string& name)
{
	
  ObjectFile* o = new ObjectFile(m_ObjectGraph,
								 on_symbol_added,
								 name, m_symbols);
  m_ObjectFiles.push_back(o);
  return o;
}

