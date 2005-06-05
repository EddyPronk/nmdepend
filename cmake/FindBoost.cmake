IF(FALSE)

FIND_PATH(Boost_ROOT_DIR include/boost-1_32/boost/utility.hpp 
		    ${Boost_ROOT_DIR} 
		    /usr/include
		    /usr/local/include
		    /opt
		    /opt/include
		    DOC "Path to Boost directory"
	)

#FIND_PATH(Boost_INCLUDE_DIR /usr/include/boost/config.hpp

ENDIF(FALSE)

SET(Boost_VER 1_31)
#SET(Boost_INCLUDE_DIR ${Boost_ROOT_DIR}/include/boost-1_32)
	    
SET(Boost_LIB_SUFFIX "gcc")


  IF(NOT HAVE_BOOST_LIBRARIES)
    EXEC_PROGRAM(${CMAKE_C_COMPILER}
      ARGS -E \"${CMAKE_SOURCE_DIR}/cmake/CheckBoostVersion.cpp\"
      OUTPUT_VARIABLE CMAKE_COMPILER_OUTPUT
      RETURN_VALUE CMAKE_COMPILER_RETURN
    )

    STRING(REGEX MATCH "version=\"[0-9_]+\"" OUTP "${CMAKE_COMPILER_OUTPUT}")
    STRING(REGEX MATCH "[0-9_]+" BOOST_VERSION "${OUTP}")
    SET(HAVE_BOOST_LIBRARIES 1)
    MESSAGE("Boost version ${BOOST_VERSION}")
  ENDIF(NOT HAVE_BOOST_LIBRARIES)


FIND_LIBRARY(Boost_Regex_LIBRARY 
  NAMES boost_regex-${Boost_LIB_SUFFIX}-${Boost_VER}
        boost_regex-${Boost_LIB_SUFFIX}
  PATHS ${Boost_ROOT_DIR}/lib
        ${Boost_ROOT_DIR}/stage/lib
	/usr/lib
  DOC "Boost Regular expressions")

FIND_LIBRARY(Boost_Filesystem_LIBRARY
  NAMES boost_filesystem-${Boost_LIB_SUFFIX}-${Boost_VER}
        boost_filesystem-${Boost_LIB_SUFFIX}
  PATHS ${Boost_ROOT_DIR}/lib
        ${Boost_ROOT_DIR}/stage/lib
	/usr/lib
  DOC "Boost Filesystem")

FIND_LIBRARY(Boost_program_options_LIBRARY
  NAMES boost_program_options-gcc 
  PATHS ${Boost_ROOT_DIR}/lib
        ${Boost_ROOT_DIR}/stage/lib
	/usr/lib
  DOC "Boost Filesystem")
