#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "ObjectPackage.h"
#include "Symbol.h"

class Archive : public ObjectPackage
{
public:
   Archive(const std::string& name);
   void Read(const char* filename, Symbol::SymbolIndex_t& symbolIndex);
   //void Link();
};

#endif

