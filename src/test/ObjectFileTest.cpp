//#include "ExampleTestCase.h"
#include <cppunit/extensions/HelperMacros.h>

#include "boost/shared_ptr.hpp"

#include "ObjectPackage.h"
#include "ObjectFile.h"
#include "SymbolStore.h"

#include <boost/ref.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_array.hpp>

using namespace std;
using boost::scoped_ptr;
using boost::scoped_array;
using boost::shared_ptr;
using boost::weak_ptr;
using boost::shared_array;

class ObjectFileTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ObjectFileTest );

#include <iostream>
using namespace std;

template<class T>
class Mock : public Callback<T>
{
public:
  typedef T* Ptr;

  typedef std::pair<Ptr, Ptr> pair;
  virtual void operator()(T& from,T& to)
  {
    m_Pair = make_pair(&from, &to);
  }
  void test(T& from, T& to)
  {
    CPPUNIT_ASSERT_EQUAL(pair(&from, &to), m_Pair);
  }
  friend std::ostream& operator<<(std::ostream& out, const pair& v)
  {
    out << *(v.first) << " " << *(v.second);
  }
private:  
  pair m_Pair;
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
    SymbolPtr q = p;
    CPPUNIT_ASSERT(p == q);

    SymbolPtr r = f.Add("func1");
    CPPUNIT_ASSERT(p == r);

    SymbolPtr s = f.Add("func2");
    CPPUNIT_ASSERT(p != s);
    
  }
  
  void linkTwoObjects()  
  {
    SymbolStore store;
    Mock<ObjectPackage> objectPackageCallback;
    Mock<ObjectFile> objectFileCallback;
    Mock<Package> packageCallback;

    Package aaa(packageCallback, "aaa");
    ObjectPackage aa(objectPackageCallback, "aa"); // superpackage of a
    aa.SetParent(aaa);
    ObjectFile a(objectFileCallback, "a.obj", store);
    a.SetParent(aa);

    a.AddImportSymbol("b");
    a.AddImportSymbol("c");
    a.AddImportSymbol("c");
    a.AddImportSymbol("d");

    Package bbb(packageCallback, "bbb");
    ObjectPackage bb(objectPackageCallback, "bb"); // superpackage of b
    bb.SetParent(bbb);
    ObjectFile b(objectFileCallback, "b.obj", store);
    b.SetParent(bb);

    b.AddExportSymbol("a");
    b.AddExportSymbol("b");
    b.AddExportSymbol("c");

    //CPPUNIT_ASSERT(!a.Depend(b));
    //CPPUNIT_ASSERT(!aa.Depend(bb));
    //CPPUNIT_ASSERT(!aaa.Depend(bbb));

    // Link all objects
    a.Link();
    b.Link();

    objectFileCallback.test(a,b);
    objectPackageCallback.test(aa,bb);
    packageCallback.test(aaa,bbb);
    
    std::set<SymbolPtr> inter;

    a.intersection(b, inter);
    CPPUNIT_ASSERT(!inter.empty());
    CPPUNIT_ASSERT(a.Depend(b));
    //CPPUNIT_ASSERT(aa.Depend(bb));
    CPPUNIT_ASSERT(aaa.Depend(bbb));
  }
};
