#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#include <map>
#include <set>
#include <string>
#include <iostream>

#include "boost/filesystem/path.hpp"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/fstream.hpp>

#include "Entity.h"
#include "Package.h"
#include "Symbol.h"

class Symbol;

class ObjectFile : public Package
{
public:
  typedef std::set<Symbol*> SymIndex_t;

  ObjectFile(const Package::Name_t& name, Symbol::SymbolIndex_t& symbolIndex);

  void Read(const boost::filesystem::path& objectfile);
  void AddImportSymbol(const std::string& name);
  void AddExportSymbol(const std::string& name);

  // Link objects like a real linker does.
  void Link();

  // Experimental operation to play with intersection
  void Boo(ObjectFile& rsh, SymIndex_t& i);

private:
   ObjectFile();

   // List of imported symbols
   SymIndex_t m_SymImports;

   // List of exported symbols.
   SymIndex_t m_SymExports;

   // For looking up in which objectfile a symbol is defined.
   Symbol::SymbolIndex_t& m_SymbolIndex;
};

#endif
