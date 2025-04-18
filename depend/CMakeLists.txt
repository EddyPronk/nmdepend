FIND_PACKAGE(Boost 1.74.0 COMPONENTS filesystem program_options regex system REQUIRED)
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
       ${Boost_LIBRARIES}
       )
