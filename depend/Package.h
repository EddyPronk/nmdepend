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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <boost/filesystem/path.hpp>

#include "Object.h"

class Package
{
  struct PackageContent
  {
    PackageContent(): refcount(0){}
    unsigned int refcount;
    std::set<Object> object;
    std::set<Package> package;
  };
public:
  typedef std::string Name_t;
  typedef std::map<Package::Name_t, Package::PackageContent > PackageRegistry;
private:
  static PackageRegistry s_PackageRegistry;
  PackageRegistry::iterator m_Package;
public:
  Package(const boost::filesystem::path& p);
  Package(const Package& p);
  ~Package();
  bool operator < (const Package& p) const;
  bool operator == (const Package& p) const;
  operator bool() const;
  const std::string& Name() const;
  const std::set<Object>& Objects() const;
  const std::set<Package>& Packages() const;
  bool Depend(const Object& o) const;
  bool Depend(const Package& p) const;
  bool PackDepend(const Package& p) const;
};


#endif
