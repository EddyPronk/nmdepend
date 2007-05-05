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

class Graph : public DependencyAddedEvent
{
public:
  typedef boost::adjacency_list<
    boost::setS,
    boost::vecS,
    boost::directedS
  > type;
  //typedef boost::graph_traits<type>::
  //  edge_parallel_category
  //  boost::disallow_parallel_edge_tag;


  void init(const std::vector<Entity*>& v)
  { 
    int size = v.size();
    for(int i = 0; i < size; ++i)
    {
      m_Indexes[v[i]] = i;
    }
  }
  void operator()(Entity& from, Entity& to)
  {
 //   assert( from.Name() != to.Name() );
    add_edge(m_Indexes[&from], m_Indexes[&to], m_Graph);
  }
  const type& get()
  {
    return m_Graph;
  }
  
private:  
  std::map<Entity*, int> m_Indexes;
  type m_Graph;
};

#endif
