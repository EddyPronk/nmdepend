# to get boost_filesystem with cygwin /cygdrive style paths build with
# bjam "-sBUILD=<define>BOOST_POSIX debug release"

SET(CMAKE_VERBOSE_MAKEFILE 1)

SET(CMAKE_CXX_FLAGS "-g3")

IF (CYGWIN)
  SET (CMAKE_CXX_FLAGS "-DBOOST_POSIX")
#  SET (CMAKE_EXE_LINKER_FLAGS "-mwindows -e _mainCRTStartup")
ENDIF (CYGWIN)

#INCLUDE (C:/Data/Ccm62/Ccm_wa/dali~Eddy/dali/Config.cmake)
#INCLUDE (C:/Data/Ccm62/Ccm_wa/dali~Eddy/dali/Reuse.cmake.txt)

#C:/Data/Ccm63/Ccm_wa/boost~1.31.0-1/boost/boost_1_31_0/stage/lib


IF (CYGWIN)
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  /cygdrive/c/Data/Ccm63/Ccm_wa/boost~1.31.0-1/boost/boost_1_31_0
)
ELSE (CYGWIN)
INCLUDE_DIRECTORIES( /home/rutger/nmdepend/src /usr/include/boost )
ENDIF (CYGWIN)

#LINK_DIRECTORIES( C:/Data/Ccm63/Ccm_wa/boost~1.31.0-1/boost/boost_1_31_0/stage/lib )

ADD_LIBRARY(nmdepend
  Entity.h Entity.cpp
  Package.h Package.cpp
  ObjectFile.h ObjectFile.cpp
  Symbol.h Symbol.cpp
)

ADD_EXECUTABLE (test1 
  test/test1.cpp
)

IF (CYGWIN)
TARGET_LINK_LIBRARIES(test1 nmdepend bfd intl iberty imagehlp)
ELSE (CYGWIN)
TARGET_LINK_LIBRARIES(test1 nmdepend bfd iberty)
ENDIF (CYGWIN)

# C:\cygwin\home\nly00711\binutils\binutils-20040312-1-modified

ADD_EXECUTABLE (test2 
  test/test2.cpp
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
TARGET_LINK_LIBRARIES(test2 nmdepend bfd intl iberty imagehlp ${Boost_filesystem})
ELSE (CYGWIN)
TARGET_LINK_LIBRARIES(test2 nmdepend bfd iberty ${Boost_filesystem})
ENDIF (CYGWIN)