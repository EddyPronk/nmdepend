#include "Graph.h"

#include <boost/graph/graphviz.hpp>

using namespace boost;
using boost::GraphvizDigraph;

void Graph::init(const std::vector<ObjectFile*>& v)
{
  int size = v.size();
  for(int i = 0; i < size; ++i)
  {
    m_Indexes[v[i]] = i;
  }
}

void Graph::operator()(ObjectFile& from,ObjectFile& to)
{
  add_edge(m_Indexes[&from], m_Indexes[&to], m_Graph);
}

Graph::type& Graph::get()
{
  return m_Graph;
}
