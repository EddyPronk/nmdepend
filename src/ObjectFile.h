#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include <map>
#include <set>
#include <string>
#include <iostream>

#include "boost/filesystem/path.hpp"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/fstream.hpp>

#include "Package.h"
#include "Symbol.h"

class ObjectFile : public Package
{
public:
  typedef std::set<Symbol*> SymIndex_t;

  ObjectFile(const Package::Name_t& name, Symbol::SymbolIndex_t& symbolIndex);

  void Read(const boost::filesystem::path& objectfile);

  void AddImportSymbol(const std::string& name);
  void AddExportSymbol(const std::string& name);
  void Link();
  void Boo(ObjectFile& rsh, SymIndex_t& i);

private:
   ObjectFile();
   SymIndex_t m_SymImports;
   SymIndex_t m_SymExports;
   Symbol::SymbolIndex_t& m_SymbolIndex;
};

#endif
