#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "ObjectFile.h"
#include "Symbol.h"

#include "bfd.h"

ObjectFile::ObjectFile(const Package::Name_t& name, Symbol::SymbolIndex_t& symbolIndex)
: Package(name), m_SymbolIndex(symbolIndex)
{
}

void ObjectFile::AddImportSymbol(const std::string& name)
{
  Symbol::SymbolIndex_t::iterator p = m_SymbolIndex.find(name);
  if (p == m_SymbolIndex.end())
  {
    Symbol* s = new Symbol(this, name);
    std::cout << "AddImportSymbol " << s->Demangled() << std::endl;
    //m_SymbolIndex[name] = s;
    m_SymImports.insert(s);
  }
  else
  {
    m_SymImports.insert(p->second);
    std::cout << "already exists : " << name << std::endl;
  }
}

void ObjectFile::AddExportSymbol(const std::string& name)
{
  Symbol::SymbolIndex_t::iterator p = m_SymbolIndex.find(name);
  if (p == m_SymbolIndex.end())
  {
    Symbol* s = new Symbol(this, name);
    std::cout << "AddExportSymbol " << s->Demangled() << std::endl;
    m_SymbolIndex[name] = s;
    m_SymExports.insert(s);
  }
  else
  {
    m_SymExports.insert(p->second);
    std::cout << "already exists : " << name << std::endl;
  }
}

void ObjectFile::Boo(ObjectFile& rsh, SymIndex_t& i)
{
  i.clear();
  set_intersection (m_SymExports.begin(), m_SymExports.end(),
              rsh.m_SymImports.begin(), rsh.m_SymImports.end(),
              std::inserter(i, i.begin()));
}

void ObjectFile::Link()
{
  for(SymIndex_t::iterator pos = m_SymImports.begin()
    ; pos != m_SymImports.end(); ++pos)
  {
    std::string& s = (*pos)->m_Name;
    Symbol::SymbolIndex_t::iterator p = m_SymbolIndex.find(s);
    if (p != m_SymbolIndex.end())
    {
      ObjectFile* owner = p->second->m_Owner;
      if (owner != this)
      {
        std::string demangled = (*pos)->Demangled();
        if (demangled.find("scalar deleting destructor") == std::string::npos)
        {
          m_Parent->AddRequires(owner);
          AddImport(owner);
          owner->AddExport(this);
          std::cout << "symbol " << s << "::" << demangled << " found in "
                    << m_SymbolIndex[s]->m_Owner->Name() << std::endl;
        }
        else
        {
          std::cout << "ignored " << Name() << "::" << demangled << " found in "
                    << m_SymbolIndex[s]->m_Owner->Name() << std::endl;
        }
      }
    }
  }
}

void ObjectFile::Read(const boost::filesystem::path& objectfile)
{
  std::cout << "file (native)" << objectfile.string() << std::endl;
  bfd *file;
  //const char* filename = "a.o";
  char *target = 0;
  file = bfd_openr (objectfile.string().c_str(), target);

  if (file == NULL)
  {
    std::cout << "can't open" << std::endl;
    // bfd_nonfatal (filename);
    //return 0;
  }

  Read(file);
}

void ObjectFile::Read(bfd* file)
{
   if (bfd_check_format (file, bfd_object))
   {
     // This is the filename with full path inside the object file.
     // Might be useful for analysing libraries.
     std::cout << "file " << bfd_get_filename (file) << std::endl;
   }

   long symcount;

   void *minisyms;

   // bfd_fffalse not in bdf.h on Cygwin and Gentoo.
   bfd_boolean dynamic = static_cast<bfd_boolean>(0); // was: bfd_fffalse;
   //struct size_sym *symsizes;
   unsigned int size;

   symcount = bfd_read_minisymbols (file, dynamic, &minisyms, &size);

   asymbol *store;
   bfd_byte *from, *fromend;

   store = bfd_make_empty_symbol (file);
   if (store == 0)
   {
     std::cout << "can't open " << std::endl;
   }

   from = (bfd_byte *) minisyms;
   fromend = from + symcount * size;
   for (; from < fromend; from += size)
   {
     asymbol* sym = bfd_minisymbol_to_symbol (file, dynamic, from, store);
     if (sym == NULL)
     {
       std::cout << "can't open" << std::endl;
     }

     const char* symname = bfd_asymbol_name (sym);

     if (sym == 0)
     {
       std::cout << "can't open" << std::endl;
     }

     if (sym->flags == 0)
     {
       AddImportSymbol(symname);
     }

     if((sym->flags & BSF_FUNCTION) != 0)
     {
       AddExportSymbol(symname);
     }

     //char *res = cplus_demangle (symname, DMGL_ANSI | DMGL_PARAMS);
     //std::cout << "symname " << sym->flags << " " << symname << std::endl;
   }
}
