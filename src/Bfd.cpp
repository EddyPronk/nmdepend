#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "Bfd.h"
#include "ObjectFile.h"

namespace fs = boost::filesystem;

Bfd::Bfd(Callback& object, Callback& package, SymbolStore& store)
 : m_ObjectGraph(object)
 , m_PackageGraph(package)
 , m_symbols(store)
{
}

Entity* Bfd::Read(const boost::filesystem::path& objectfile)
{
  fs::path::iterator p = objectfile.end();
  --p;
  std::string name = *p;
  ObjectFile* file = new ObjectFile(m_ObjectGraph, name, m_symbols);
  file->Read(objectfile);
  return file;
}
