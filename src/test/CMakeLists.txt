FIND_PACKAGE(CppUnit)
FIND_PACKAGE(Perl)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CppUnit_INCLUDE_DIR})

ADD_EXECUTABLE (cppunit Main.cpp ObjectFileTest.cpp NewTest.cpp)
TARGET_LINK_LIBRARIES(cppunit core ${CppUnit_LIBRARY})

ADD_TEST("CppUnit" cppunit)
ADD_TEST("SmokeTest" ../nmdepend --help)
ADD_TEST("CheckOutputProject1" ${PERL_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/checkOutput.pl project1 ${CMAKE_CURRENT_SOURCE_DIR} 3)

ADD_SUBDIRECTORY(project1)

# project2 was intended to test the -n feature.
# this needs a slightly different build tree.
# current cmake produces an identical tree 

#ADD_SUBDIRECTORY(project2)
#ADD_TEST("CheckOutputProject2" ${PERL_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/checkOutput.pl project2 ${CMAKE_CURRENT_SOURCE_DIR} 4)

