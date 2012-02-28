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

#include "Package.h"

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>

namespace fs = boost::filesystem;

Package::PackageRegistry Package::s_PackageRegistry;

Package::Package(const fs::path& p)
{
  m_Package = s_PackageRegistry.find(p.string()/*.leaf()*/);

  if(m_Package == s_PackageRegistry.end())
  {
    std::pair<PackageRegistry::iterator, bool> ret =
      s_PackageRegistry.insert(
	std::make_pair(p.string()/*.leaf()*/, PackageContent())
      );
    if(ret.second)
    {
      m_Package = ret.first;
    }
  }

  ++m_Package->second.refcount;

  fs::directory_iterator end;
  for(fs::directory_iterator it(p); it != end; ++it)
  {
    if(fs::is_directory(*it))
    {
      Package pack(*it);
      if(pack)
      {
	m_Package->second.package.insert(pack);
      }
    }
    else
    {
      if(fs::extension(*it) == ".o")
      {
	Object o(*it);
        m_Package->second.object.insert(o);
      }
    }
  }
  if(m_Package->second.object.empty() && m_Package->second.package.empty())
  {
    s_PackageRegistry.erase(m_Package);
    m_Package = s_PackageRegistry.end();
  }
}
Package::Package(const Package& p):
  m_Package(p.m_Package)
{
  ++m_Package->second.refcount;
}
Package::~Package()
{
  if(--m_Package->second.refcount == 0)
  {
    s_PackageRegistry.erase(m_Package);
  }
}
bool Package::operator < (const Package& p) const
{
  return m_Package->first < p.m_Package->first;
}
bool Package::operator == (const Package& p) const
{
  return m_Package->first == p.m_Package->first;
}
Package::operator bool() const
{
  return m_Package != s_PackageRegistry.end();
}
const std::string& Package::Name() const
{
  return m_Package->first;
}
const std::set<Object>& Package::Objects() const
{
  return m_Package->second.object;
}
const std::set<Package>& Package::Packages() const
{
  return m_Package->second.package;
}
bool Package::Depend(const Object& o) const
{
  bool depends(false);

  for(std::set<Object>::const_iterator it = m_Package->second.object.begin();
      it != m_Package->second.object.end(); ++it)
  {
    if(it->Depend(o))
    {
      //direct dependency
      depends = true;
    }
  }
  
  for(std::set<Package>::const_iterator it = m_Package->second.package.begin();
      it != m_Package->second.package.end(); ++it)
  {
     it->Depend(o);
  }

  return depends;
}
bool Package::Depend(const Package& p) const
{
  bool depends(false);

  std::string cluster(fs::basename(fs::path(m_Package->first).leaf()));
  if(cluster.empty())
  {
    cluster = fs::basename(fs::absolute(m_Package->first).filename());
  }

  std::cout << "subgraph cluster_"
            << cluster
            << "{" << std::endl
            << "label=" << cluster
            << ';' << std::endl;

  for(std::set<Object>::const_iterator it = m_Package->second.object.begin();
      it != m_Package->second.object.end(); ++it)
  {
    std::cout << '"' << fs::basename(fs::path(it->Name()).leaf()) << "\";" << std::endl;
  }

  for(std::set<Package>::const_iterator it = m_Package->second.package.begin();
      it != m_Package->second.package.end(); ++it)
  {
    it->Depend(p);
  }

  std::cout << "}" << std::endl;

  return depends;
}

bool Package::PackDepend(const Package& p) const
{
  bool depends(false);

  Depend(p);

  for(std::set<Package>::const_iterator it = m_Package->second.package.begin();
      it != m_Package->second.package.end(); ++it)
  {
    p.PackDepend(*it);
  }

  for(std::set<Object>::const_iterator it = m_Package->second.object.begin();
      it != m_Package->second.object.end(); ++it)
  {
    if(it->Depend(p))
    {
      //direct dependency
      depends = true;
    }
  }

  if(depends && Name() != p.Name())
  {
    std::clog << '"' << fs::basename(fs::path(Name()).leaf()) << "\" -> \""
              << fs::basename(fs::path(p.Name()).leaf()) << "\";" << std::endl;
  }
  return depends;
}
