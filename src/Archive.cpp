#include "Archive.h"
#include "ObjectFile.h"

#include "bfd.h"
#include <iostream>

Archive::Archive(const Name_t& name) : ObjectPackage(name)
{
}

void Archive::Read(const char* filename, Symbol::SymbolIndex_t& symbolIndex)
{
   char *target = 0;
   bfd *file;

   file = bfd_openr (filename, target);
   if (file == 0)
   {
      //    bfd_nonfatal (filename);
   //   return FALSE;
   }

   if (bfd_check_format (file, bfd_archive))
   {
      //    display_archive (file);
      bfd *arfile = 0;

      std::cout << "file " << bfd_get_filename (file) << std::endl;

      while(arfile = bfd_openr_next_archived_file (file, arfile))
      {
         const char* objectname = bfd_get_filename (arfile);
         ObjectFile* o = new ObjectFile(objectname, symbolIndex);
         o->SetParent(*this);
         o->Read(arfile);
         std::cout << "file " << bfd_get_filename (arfile) << std::endl;
      }

      if (arfile == NULL)
      {
      //if (bfd_get_error () != bfd_error_no_more_archived_files)
      //bfd_fatal (bfd_get_filename (file));
      //break;
      }
   }
}

