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

#ifndef GRAPH_H
#define GRAPH_H

#include <boost/graph/adjacency_list.hpp>
//#include <boost/property_map.hpp>
#include <boost/graph/graphviz.hpp>

#include <vector>
#include <map>

#include "Callback.h"

class ObjectFile;

class Graph : public Callback<ObjectFile>
{
public:
  typedef boost::adjacency_list<> type;

  void init(const std::vector<ObjectFile*>& v);
  virtual void operator()(ObjectFile& from,ObjectFile& to);
  type& get();
private:  
  std::map<ObjectFile*, int> m_Indexes;
  type m_Graph;
};

#endif
