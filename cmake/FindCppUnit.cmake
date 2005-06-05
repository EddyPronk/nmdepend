FIND_LIBRARY(CppUnit_LIBRARY 
  NAMES cppunit 
  PATHS /usr/lib
        /usr/local/lib
       
  ${CppUnit_ROOT_DIR}//lib
  DOC "CppUnit test framework")
  
SET(CppUnit_INCLUDE_DIR ${CppUnit_ROOT_DIR}/include) 
