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
  CPPUNIT_TEST( example );
  CPPUNIT_TEST( testSymbolStore );
  CPPUNIT_TEST( test1 );
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

  void example()
  {
    SymbolStore store;
    
    ObjectPackage aa("aa"); // superpackage of a
    ObjectFile a("a.obj", store);
    ObjectFile b("b.obj", store);
    a.SetParent(aa);
    a.AddImportSymbol("func1");
    b.AddExportSymbol("func1");
    CPPUNIT_ASSERT(true);
  }

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
  
  void test1()  
  {
    SymbolStore store;

    ObjectPackage aa("aa"); // superpackage of a
    ObjectFile a("a.obj", store);
    a.SetParent(aa);

    a.AddImportSymbol("b");
    a.AddImportSymbol("c");
    a.AddImportSymbol("c");
    a.AddImportSymbol("d");

    ObjectPackage bb("bb"); // superpackage of b
    ObjectFile b("b.obj", store);
    b.SetParent(bb);

    b.AddExportSymbol("a");
    b.AddExportSymbol("b");
    b.AddExportSymbol("c");

    // Link all objects
    a.Link();
    b.Link();

    std::set<SymbolPtr> inter;

    a.intersection(b, inter);
    CPPUNIT_ASSERT(!inter.empty());
    CPPUNIT_ASSERT(a.Depend(b));
  }
};
