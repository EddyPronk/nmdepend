FIND_PATH(Boost_ROOT_DIR include/boost-1_32/boost/utility.hpp 
		    ${Boost_ROOT_DIR} 
		    /usr/include
		    /usr/local/include
		    /opt
		    /opt/include
		    DOC "Path to Boost directory"
	)

SET(Boost_VER 1_32)
SET(Boost_INCLUDE_DIR ${Boost_ROOT_DIR}/include/boost-1_32)
	    
SET(Boost_LIB_SUFFIX "gcc")

FIND_LIBRARY(Boost_Regex_LIBRARY 
  NAMES boost_regex-${Boost_LIB_SUFFIX}-${Boost_VER}
        boost_regex-${Boost_LIB_SUFFIX}
  PATHS ${Boost_ROOT_DIR}/lib
        ${Boost_ROOT_DIR}/stage/lib
  DOC "Boost Regular expressions")

FIND_LIBRARY(Boost_Filesystem_LIBRARY
  NAMES boost_filesystem-${Boost_LIB_SUFFIX}-${Boost_VER}
        boost_filesystem-${Boost_LIB_SUFFIX}
  PATHS ${Boost_ROOT_DIR}/lib
        ${Boost_ROOT_DIR}/stage/lib
  DOC "Boost Filesystem")

FIND_LIBRARY(Boost_program_options_LIBRARY
  NAMES boost_program_options-gcc 
  PATHS ${Boost_ROOT_DIR}/lib
        ${Boost_ROOT_DIR}/stage/lib
  DOC "Boost Filesystem")
