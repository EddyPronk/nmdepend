# to get boost_filesystem with cygwin /cygdrive style paths build with
# bjam "-sBUILD=<define>BOOST_POSIX debug release"

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)

#SET(CMAKE_MODULE_PATH /home/nmdepend/cmake)

SET(INCLUDES "")
CHECK_INCLUDE_FILES("${INCLUDES};bfd.h" HAVE_BFD_H)
SET(CMAKE_VERBOSE_MAKEFILE 1)

#INCLUDE(../cmake/FindBinutils.cmake)
FIND_PACKAGE(Boost)
FIND_PACKAGE(CommonC++)
FIND_PACKAGE(Binutils)
FIND_PACKAGE(Zlib)

#SET(CMAKE_CXX_FLAGS "-g3") # huh?
SET(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")


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
  Entity.h Entity.cpp
  Package.h Package.cpp
  ObjectFile.h ObjectFile.cpp
  ObjectPackage.h ObjectPackage.cpp
  Archive.h Archive.cpp
  Symbol.h Symbol.cpp
  SymbolParser.h SymbolParser.cpp
  )

SET(Nmdepend_LIBRARIES
	${BFD_LIBRARY}
	${INTL_LIBRARY}
 	${IBERTY_LIBRARY}
 	${Boost_Filesystem_LIBRARY}
 	${Boost_Regex_LIBRARY}
	${CommonC++_ccext2_LIBRARY}
	${CommonC++_ccgnu2_LIBRARY}
        ${ZLIB_LIBRARY}
    #    /usr/lib/libdl.a
 	)

IF (CYGWIN)
	SET(Nmdepend_LIBRARIES ${Nmdepend_LIBRARIES} imagehlp)
ENDIF (CYGWIN)

ADD_EXECUTABLE(nmdepend Nmdepend.cpp)
TARGET_LINK_LIBRARIES(nmdepend core ${Nmdepend_LIBRARIES})

ADD_EXECUTABLE (test1 test/test1.cpp)
TARGET_LINK_LIBRARIES(test1 core ${Nmdepend_LIBRARIES})

ADD_EXECUTABLE (test2 test/test2.cpp)
TARGET_LINK_LIBRARIES(test2 core ${Nmdepend_LIBRARIES})

ADD_EXECUTABLE (test4 test/test4.cpp)
TARGET_LINK_LIBRARIES(test4 core ${Nmdepend_LIBRARIES})

ADD_TEST("smoke" nmdepend --help)
ADD_TEST("run-on-example" nmdepend ../example)


INSTALL_TARGETS(/bin nmdepend)
