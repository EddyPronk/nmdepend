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

#include <iostream>
#include <algorithm>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "Graph.h"
#include "Callback.h"
#include "ObjectPackage.h"
#include "ObjectFile.h"
#include "Bfd.h"
#include "Factory.h"

namespace fs = boost::filesystem;
using namespace std;

class CallBackDummy : public Callback
{
public:
  typedef Entity* Ptr;

  typedef std::pair<Ptr, Ptr> pair;
  virtual void operator()(Entity& from, Entity& to)
  {
    cout << from << " -> " << to << endl;
  }
};

template<class T>
struct wrapper
{
  wrapper(vector<T*>& v) : m_v(v) {}
  const string& operator[](int i) const
  {
    return m_v[i]->Name();
  }
  const vector<T*>& m_v;
};

struct sample_graph_writer
{
  void operator()(std::ostream& out) const
  {
    out << "node [shape=box]" << std::endl;
  }
};

class Analyser
{
public:
   typedef std::vector<fs::path> filelist_t;

   Analyser(int packageLevel);
   
   // todo : match wildcards with filter_iterator and regex (*.o) (*.a) (*.lib)
   void find_file( const fs::path& dir_path);
   void Add(const std::vector<string>& files);
   void AddPackages(const std::vector<string>& packages);
   void ReadObjects();
   void Link();
   void WriteObjectGraph(std::ostream& out);
   void WritePackageGraph(std::ostream& out);
   
private:
   Bfd m_Bfd;
   Factory m_Factory;
   filelist_t list;
   Graph m_ObjectGraph;
   Graph m_PackageGraph;
   //std::set<ObjectPackage> m_PackageSet;
   std::vector<Entity*> m_Packages;
   //std::vector<Entity*> m_ObjectFiles;
   int m_packageLevel;
};
