#include "Package.cpp"

Package::AddImport(IPackage& p)
{
   m_Imports.push_back(p);
}