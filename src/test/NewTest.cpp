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
#include <cppunit/extensions/HelperMacros.h>

#include <cc++/common.h>

#include "ObjectFile.h"
#include "ObjectPackage.h"
#include "SymbolStore.h"
#include "ObjectPackage.h"
#include "ObjectFile.h"
#include "SymbolStore.h"
#include "Analyser.h"
#include "Graph.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

namespace fs = boost::filesystem;

using namespace std;


class NewTest;

CPPUNIT_TEST_SUITE_REGISTRATION( NewTest );

using namespace std;


using boost::GraphvizDigraph;

class NewTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( NewTest );
  CPPUNIT_TEST( newTest );
  CPPUNIT_TEST( newTest2 );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
  }

protected:

  void newTest()  
  {
    Analyser a;
    fs::path exampleDir = fs::initial_path() / "../../example";
    exampleDir.normalize();
    a.find_file(exampleDir);
    a.ReadObjects();
  }

  void newTest2()  
  {
    Graph<ObjectFile> g;
    SymbolStore store;
    ObjectFile b(g, "b.obj", store);
    ObjectFile f(g, "f.obj", store);
    ObjectFile h(g, "h.obj", store);
    vector<ObjectFile*> m_ObjectFiles;
    m_ObjectFiles.push_back(&b);
    m_ObjectFiles.push_back(&f);
    m_ObjectFiles.push_back(&h);
    
    g.init(m_ObjectFiles);
    g(b,f);
    g(f,h);
    g(f,h); // add the same dependency a 2nd time
    g(b,h);
    
    wrapper<ObjectFile> w(m_ObjectFiles);
    boost::write_graphviz(cout, g.get(),
      boost::make_label_writer(w));
  }
};
