# to get boost_filesystem with cygwin /cygdrive style paths build with
# bjam "-sBUILD=<define>BOOST_POSIX debug release"

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)

#SET(CMAKE_MODULE_PATH /home/nmdepend/cmake)

SET(INCLUDES "")
CHECK_INCLUDE_FILES("${INCLUDES};bfd.h" HAVE_BFD_H)
CHECK_INCLUDE_FILES("${INCLUDES};demangle.h" HAVE_DEMANGLE_H)
SET(CMAKE_VERBOSE_MAKEFILE 1)

#INCLUDE(../cmake/FindBinutils.cmake)
FIND_PACKAGE(Boost)
FIND_PACKAGE(CommonC++)
FIND_PACKAGE(Binutils)
FIND_PACKAGE(ZLIB)

#SET(CMAKE_CXX_FLAGS "-g3") # huh?
SET(CMAKE_CXX_FLAGS "-O2 -fprofile-arcs -ftest-coverage")


IF (CYGWIN)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_POSIX")
#  SET (CMAKE_EXE_LINKER_FLAGS "-mwindows -e _mainCRTStartup")
#  SET (CMAKE_EXE_LINKER_FLAGS "-mno-cygwin")
ENDIF (CYGWIN)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src
      	${Boost_INCLUDE_DIR}
      	${CommonC++_INCLUDE_DIR}
	${Binutils_INCLUDE_DIR}
)

ADD_LIBRARY(core
  Entity.cpp
  Package.cpp
  ObjectFile.cpp
  ObjectPackage.cpp
#  Archive.cpp
  Symbol.cpp
  SymbolStore.cpp
  Graph.cpp
#  SymbolParser.cpp
  )

TARGET_LINK_LIBRARIES(core
	${BFD_LIBRARY}
#	${INTL_LIBRARY}
 	${IBERTY_LIBRARY}
 	${Boost_Filesystem_LIBRARY}
# 	${Boost_Regex_LIBRARY}
	${CommonC++_ccext2_LIBRARY}
	${CommonC++_ccgnu2_LIBRARY}
        /usr/lib/libdl.a
        ${ZLIB_LIBRARIES})

IF(CYGWIN)
  TARGET_LINK_LIBRARIES(core /bin/cygintl-2.dll)
ENDIF (CYGWIN)

ADD_EXECUTABLE(nmdepend Nmdepend.cpp)
TARGET_LINK_LIBRARIES(nmdepend core)

SUBDIRS(test)

INSTALL_TARGETS(/bin nmdepend)
