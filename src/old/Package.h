#ifndef PACKAGE_H
#define PACKAGE_H

class Package : public IPackage
{
public:
   //typedef std::vector<IPa
   Package(const IPackage::Name_t& name)
      : IPackage(name)
   {
   }

private:
   Index_t m_Imports;
   Index_t m_Exports;
};

#endif
