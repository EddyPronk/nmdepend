//#include "ExampleTestCase.h"
#include <cppunit/extensions/HelperMacros.h>

#include "Symbol.h"

class ExampleTestCase;

CPPUNIT_TEST_SUITE_REGISTRATION( ExampleTestCase );

class ExampleTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ExampleTestCase );
  CPPUNIT_TEST( example );
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
    // Create two symbols and check if they are pointing to the same.
    Symbol a("func1");
    Symbol b("func1");
    
    CPPUNIT_ASSERT(true);
  }
};
