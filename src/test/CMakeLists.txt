FIND_PACKAGE(CppUnit)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CppUnit_INCLUDE_DIR})

ADD_EXECUTABLE (cppunit Main.cpp ObjectFileTest.cpp NewTest.cpp)
TARGET_LINK_LIBRARIES(cppunit core ${Nmdepend_LIBRARIES} ${CppUnit_LIBRARY})

ADD_TEST("CppUnit" cppunit)