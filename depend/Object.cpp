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


#include "Object.h"
#include "Package.h"

#include <bfd.h>

#include <boost/filesystem/convenience.hpp>

#include <iostream>

namespace fs = boost::filesystem;

Object::ObjectRegistry Object::s_ObjectRegistry;

Object::Object(const fs::path& p)
{
  m_Object = s_ObjectRegistry.find(fs::basename(p));
  if(m_Object == s_ObjectRegistry.end())
  {
    std::pair<ObjectRegistry::iterator, bool>
      ret = s_ObjectRegistry.insert(std::make_pair(p.string()/*fs::basename(p)*/, ImportExport()));

    if(ret.second)
    {
      m_Object = ret.first;
    }
  }

  ++m_Object->second.refcount;

  assert(m_Object != s_ObjectRegistry.end());

  char *target = 0;
  bfd *file = bfd_openr (p.native_file_string().c_str(), target);
  if (file == NULL)
  {
    std::clog << "error opening: " << p.string() << std::endl;
  }
  else
  {
    if(bfd_check_format (file, bfd_object))
    {
      void* minisyms;
      bfd_boolean dynamic(static_cast<bfd_boolean>(0));
      unsigned int size;
      long  symcount = bfd_read_minisymbols (file, dynamic, &minisyms, &size);

      asymbol*  store;
      bfd_byte* from;
      bfd_byte* fromend;

      store = bfd_make_empty_symbol(file);
      if (store == 0)
      {
        std::clog << "can't open " << std::endl;
      }
      else
      {
        from = static_cast<bfd_byte*>(minisyms);
        fromend = from + symcount * size;

        for (;from < fromend; from += size)
        {
          asymbol* sym = bfd_minisymbol_to_symbol (file, dynamic, from, store);

          if (sym == 0)
          {
            std::clog << "can't open" << std::endl;
          }
          else
          {
            std::string symname(bfd_asymbol_name(sym));

            if (sym->flags == 0)
            {
              Import(symname);
            }

            if((sym->flags & BSF_GLOBAL) != 0)
            {
              Export(symname);
            }
          }
        }      
      }
    }
    bfd_close(file);
  }
}
Object::Object(const Object& o):
  m_Object(o.m_Object)
{
  ++m_Object->second.refcount;
}
Object::~Object()
{
  if(--m_Object->second.refcount == 0)
  {
    s_ObjectRegistry.erase(m_Object);
  }
}
bool Object::operator < (const Object& rhs) const
{
  return  m_Object->first < rhs.m_Object->first;
}
bool Object::operator == (const Object& rhs) const
{
  return  m_Object->first == rhs.m_Object->first;
}
bool Object::operator != (const Object& rhs) const
{
  return  m_Object->first != rhs.m_Object->first;
}
void Object::Import(const Symbol& s)
{
  m_Object->second.Import.insert(s);
}
void Object::Export(const Symbol& s)
{
  m_Object->second.Export.insert(s);
}
const std::string& Object::Name() const
{
  return m_Object->first;
}
bool Object::Depend(const Symbol& s) const
{
  return m_Object->second.Import.find(s) != m_Object->second.Import.end();
}

bool Object::Depend(const Object& o) const
{
  const std::set<Symbol>& imprt(m_Object->second.Import);
  const std::set<Symbol>& exprt(o.m_Object->second.Export);

  std::set<Symbol> intersect;
  std::insert_iterator<std::set<Symbol> > inserter(intersect,intersect.begin());

  std::set_intersection(imprt.begin(),
                        imprt.end(),
                        exprt.begin(),
			exprt.end(),
			inserter);
  if(!intersect.empty())
  {
    try
    {
      std::cout << '"' << fs::basename(fs::path(m_Object->first).leaf())
                << "\" -> \""
                << fs::basename(fs::path(o.m_Object->first).leaf()) << "\";"
                << std::endl;
    }
    catch(std::exception& e)
    {
      std::clog << "caught exception: " << e.what() << " in " << __FILE__
                << " at: " << __LINE__ << std::endl;
    }
  }

  return !intersect.empty();
}
bool Object::Depend(const Package& p) const
{
  bool depends(false);

  for(std::set<Object>::const_iterator it = p.Objects().begin();
      it != p.Objects().end(); ++it)
  {
    //tests it->Depend(*it) if and only if *this != *it
    if(*this != *it && Depend(*it))
    {
      //direct dependency
      depends = true;
    }
  }
  
  for(std::set<Package>::const_iterator it = p.Packages().begin();
      it != p.Packages().end(); ++it)
  {
    Depend(*it);
  }

  return depends;
}
