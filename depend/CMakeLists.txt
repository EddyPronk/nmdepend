
SET(CMAKE_VERBOSE_MAKEFILE 1)

ADD_EXECUTABLE (depend
  depend.cpp
  Object.cpp
  Package.cpp
  Symbol.cpp
)

TARGET_LINK_LIBRARIES(depend bfd iberty boost_filesystem boost_regex)
