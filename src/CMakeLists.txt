# to get boost_filesystem with cygwin /cygdrive style paths build with
# bjam "-sBUILD=<define>BOOST_POSIX debug release"

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)

SET(INCLUDES "")
CHECK_INCLUDE_FILES("${INCLUDES};bfd.h" HAVE_BFD_H)
SET(CMAKE_VERBOSE_MAKEFILE 1)


SET(CMAKE_CXX_FLAGS "-g3") # huh?

IF (CYGWIN)
  SET (CMAKE_CXX_FLAGS "-DBOOST_POSIX")
#  SET (CMAKE_EXE_LINKER_FLAGS "-mwindows -e _mainCRTStartup")
ENDIF (CYGWIN)

IF (CYGWIN)
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  /cygdrive/c/Data/Ccm63/Ccm_wa/boost~1.31.0-1/boost/boost_1_31_0
  /usr/local/include/cc++2
)
ELSE (CYGWIN)
  INCLUDE_DIRECTORIES( /home/rutger/nmdepend/src /usr/include/boost
  )
ENDIF (CYGWIN)

#LINK_DIRECTORIES( C:/Data/Ccm63/Ccm_wa/boost~1.31.0-1/boost/boost_1_31_0/stage/lib )

ADD_LIBRARY(core
  Entity.h Entity.cpp
  Package.h Package.cpp
  ObjectFile.h ObjectFile.cpp
  ObjectPackage.h ObjectPackage.cpp
  Archive.h Archive.cpp
  Symbol.h Symbol.cpp
)

ADD_EXECUTABLE (nmdepend
  Nmdepend.cpp
  #cmdlineopt.cpp
)

FIND_LIBRARY(ccext2
  ccext2
  /usr/lib
  /usr/local/lib
  )

FIND_LIBRARY(ccgnu2
  ccgnu2
  /usr/lib
  /usr/local/lib
  )

TARGET_LINK_LIBRARIES(nmdepend core bfd intl iberty imagehlp
${ccgnu2} ${ccext2} ${Boost_filesystem})


ADD_EXECUTABLE (test1
  test/test1.cpp
)

IF (CYGWIN)
TARGET_LINK_LIBRARIES(test1 core bfd intl iberty imagehlp)
ELSE (CYGWIN)
TARGET_LINK_LIBRARIES(test1 core bfd iberty)
ENDIF (CYGWIN)

# C:\cygwin\home\nly00711\binutils\binutils-20040312-1-modified

ADD_EXECUTABLE (test2
  test/test2.cpp
)

ADD_EXECUTABLE (test4
  test/test4.cpp
)

IF (CYGWIN)
FIND_LIBRARY(Boost_filesystem
  boost_filesystem-gcc-1_31
  /cygdrive/c/Data/Ccm63/Ccm_wa/boost~1.31.0-1/boost/boost_1_31_0/stage/lib
  )
ELSE (CYGWIN)
FIND_LIBRARY(Boost_filesystem
  boost_filesystem
  /usr/lib
  )
ENDIF (CYGWIN)

IF (CYGWIN)
TARGET_LINK_LIBRARIES(test2 core bfd intl iberty imagehlp ${Boost_filesystem})
TARGET_LINK_LIBRARIES(test4 core bfd intl iberty imagehlp ${Boost_filesystem})
ELSE (CYGWIN)
TARGET_LINK_LIBRARIES(test2 core bfd iberty ${Boost_filesystem})
ENDIF (CYGWIN)

INSTALL_TARGETS(/bin nmdepend)
