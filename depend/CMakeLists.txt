SET(CMAKE_MODULE_PATH /home/nmdepend/cmake)

FIND_PACKAGE(Boost)
FIND_PACKAGE(Binutils)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ../src
	${Binutils_INCLUDE_DIR})

SET(CMAKE_VERBOSE_MAKEFILE 1)

ADD_EXECUTABLE (depend
  depend.cpp
  Object.cpp
  Package.cpp
  Symbol.cpp
)

TARGET_LINK_LIBRARIES(depend
       ${BFD_LIBRARY}
       ${INTL_LIBRARY}
       ${IBERTY_LIBRARY}
       ${Boost_Filesystem_LIBRARY}
       ${Boost_Regex_LIBRARY}
       )
