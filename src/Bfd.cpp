#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "bfd.h"
#include "Bfd.h"
#include "ObjectFile.h"
#include "Factory.h"

namespace fs = boost::filesystem;

Bfd::Bfd(Factory& factory)
 : m_Factory(factory)
{
}

Entity* Bfd::Read(const boost::filesystem::path& path, int packageLevel)
{
  char *target = 0;
  bfd* file = bfd_openr (path.string().c_str(), target);
  assert(file);

  Entity* object = NULL;

  fs::path::iterator p = path.end();
  --p;
  std::string name = *p;
 
  if (bfd_check_format (file, bfd_object))
  {
    --p;
    if(packageLevel == 2)
    {
      --p;
    }
    std::string packagename = *p;
  
    ObjectPackage* op = m_Factory.CreatePackage(packagename);
    assert(op);
    ObjectFile* of = m_Factory.CreateObject(name);
    of->Read(file);
    of->SetParent(*op);
    object = op;
  }

  if (bfd_check_format (file, bfd_archive))
  {
    ObjectPackage* package = m_Factory.CreatePackage(name);
    object = package;
    bfd *arfile = 0;

    std::cout << "file " << bfd_get_filename (file) << std::endl;

    while((arfile = bfd_openr_next_archived_file (file, arfile)))
    {
      const char* objectname = bfd_get_filename (arfile);
      ObjectFile* o = m_Factory.CreateObject(objectname);
      o->SetParent(*package);
      o->Read(arfile);
      std::cout << "file " << bfd_get_filename (arfile) << std::endl;
    }
  }

  return object;
}
