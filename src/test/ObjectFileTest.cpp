//#include "ExampleTestCase.h"
#include <cppunit/extensions/HelperMacros.h>

#include "boost/shared_ptr.hpp"

#include "ObjectPackage.h"
#include "ObjectFile.h"
#include "SymbolStore.h"

class ObjectFileTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ObjectFileTest );

#include <iostream>
using namespace std;

class A
{
  public:
  A()
  {
    cerr << "A::A()" << endl;
  }

  ~A()
  {
    cerr << "A::~A()" << endl;
  }
};

class ObjectFileTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ObjectFileTest );
  CPPUNIT_TEST( testSymbolStore );
  CPPUNIT_TEST( linkTwoObjects );
  CPPUNIT_TEST_SUITE_END();

protected:
  double m_value1;
  double m_value2;

public:
  void setUp()
  {
    m_value1 = 2.0;
    m_value2 = 3.0;
  }

protected:

  void testSymbolStore()  
  {
    SymbolStore f;

    SymbolPtr p = f.Add("func1");
    cout << p->Name() << endl;
    SymbolPtr q = p;
    CPPUNIT_ASSERT(p == q);

    cout << q->Name() << endl;
    SymbolPtr r = f.Add("func1");
    cout << r->Name() << endl;
    CPPUNIT_ASSERT(p == r);

    SymbolPtr s = f.Add("func2");
    cout << r->Name() << endl;
    CPPUNIT_ASSERT(p != s);
    
  }
  
  void linkTwoObjects()  
  {
    SymbolStore store;

    Package aaa("aaa");
    ObjectPackage aa("aa"); // superpackage of a
    aa.SetParent(aaa);
    ObjectFile a("a.obj", store);
    a.SetParent(aa);

    a.AddImportSymbol("b");
    a.AddImportSymbol("c");
    a.AddImportSymbol("c");
    a.AddImportSymbol("d");

    Package bbb("bbb");
    ObjectPackage bb("bb"); // superpackage of b
    bb.SetParent(bbb);
    ObjectFile b("b.obj", store);
    b.SetParent(bb);

    b.AddExportSymbol("a");
    b.AddExportSymbol("b");
    b.AddExportSymbol("c");

    CPPUNIT_ASSERT(!a.Depend(b));
    CPPUNIT_ASSERT(!aa.Depend(bb));
    CPPUNIT_ASSERT(!aaa.Depend(bbb));

    // Link all objects
    a.Link();
    b.Link();

    std::set<SymbolPtr> inter;

    a.intersection(b, inter);
    CPPUNIT_ASSERT(!inter.empty());
    CPPUNIT_ASSERT(a.Depend(b));
    CPPUNIT_ASSERT(aa.Depend(bb));
    CPPUNIT_ASSERT(aaa.Depend(bbb));
  }
};
