#include <set>
#include "SymbolStore.h"
#include <iostream>

using namespace std;

SymbolPtr SymbolStore::Add(const std::string& name)
{
  std::pair<std::set<Symbol>::iterator,bool> status = m_Symbols.insert(name);
  return (SymbolPtr)&(*(status.first));
}
