// Copyright (c) 2004 E.J. Pronk and R.E.W. van Beusekom
//
// This file is part of Nmdepend.
// 
// Nmdepend is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// Nmdepend is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Nmdepend; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

