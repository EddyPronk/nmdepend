#include <set>
#include "SymbolStore.h"
#include <iostream>

using namespace std;

SymbolPtr SymbolStore::Add(const std::string& name)
{
  std::pair<std::set<Symbol>::iterator,bool> status = m_Symbols.insert(name);
  return (SymbolPtr)&(*(status.first));
}

void SymbolStore::Statistics()
{
  int total = m_Symbols.size();
  cout << total << " symbols" << endl;

  int counter = 0;
  for(std::set<Symbol>::iterator pos = m_Symbols.begin();
      pos != m_Symbols.end(); ++pos)
  {
    if( pos->foundOwner())
    {
      ++counter;
    }
  }
  
  if(total != counter)
  {
    cout << counter << " linked" << endl;
    cout << total - counter << " unresolved" << endl;
  }
}
