#ifndef BFD_H
#define BFD_H_

#include <boost/filesystem/path.hpp>
//#include <boost/filesystem.hpp>
#include "Entity.h"
#include "Symbol.h"
#include "SymbolStore.h"
#include "Graph.h"
#include "Callback.h"

class Bfd
{
public:
//  Bfd(Callback&, const std::string& name, SymbolStore& store);i
  Bfd(Callback& object, Callback& package, SymbolStore& store);
  Entity* Read(const boost::filesystem::path& objectfile);
private:
  SymbolStore& m_symbols;
  Callback& m_ObjectGraph;
  Callback& m_PackageGraph;
};

#endif
