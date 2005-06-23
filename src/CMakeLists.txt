# to get boost_filesystem with cygwin /cygdrive style paths build with
# bjam "-sBUILD=<define>BOOST_POSIX debug release"

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)

SET(INCLUDES "")
CHECK_INCLUDE_FILES("${INCLUDES};bfd.h" HAVE_BFD_H)
CHECK_INCLUDE_FILES("${INCLUDES};demangle.h" HAVE_DEMANGLE_H)

FIND_PACKAGE(Binutils)
FIND_PACKAGE(ZLIB)
FIND_PACKAGE(Boost)
BOOST_USE(filesystem)
BOOST_USE(program_options)


SET(CMAKE_CXX_FLAGS "-Wall -W -O2")

IF(COVERAGE)
 # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF(COVERAGE)

IF (CYGWIN)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_POSIX")
ENDIF (CYGWIN)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIR}
#    ${CommonC++_INCLUDE_DIR}
#	${Binutils_INCLUDE_DIR}
)

ADD_LIBRARY(core
  Entity.cpp
  Bfd.cpp
  Factory.cpp
  ObjectFile.cpp
  ObjectPackage.cpp
  Symbol.cpp
  SymbolStore.cpp
  Graph.cpp
  Analyser.cpp
  )

TARGET_LINK_LIBRARIES(core
	${BFD_LIBRARY}
 	${IBERTY_LIBRARY}
 	${Boost_filesystem_LIBRARY}
    ${Boost_program_options_LIBRARY}
)

IF(CYGWIN)
  TARGET_LINK_LIBRARIES(core /bin/cygintl-2.dll)
ELSE(CYGWIN)
  TARGET_LINK_LIBRARIES(core ${ZLIB_LIBRARIES} /usr/lib/libdl.a)
ENDIF (CYGWIN)

ADD_EXECUTABLE(nmdepend main.cpp)
TARGET_LINK_LIBRARIES(nmdepend core)

IF(BUILD_TESTING)
  SUBDIRS(test)
ENDIF(BUILD_TESTING)

INSTALL_TARGETS(/bin nmdepend)
