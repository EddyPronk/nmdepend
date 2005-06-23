FIND_PATH(Boost_INCLUDE_DIR boost/config.hpp
	/usr/include
	/usr/local/include/boost-1_32/
	)

SET(Boost_LIB_SUFFIX "gcc")

MACRO(BOOST_USE name)
  MESSAGE("-- Use ${name}")
  FIND_LIBRARY(Boost_${name}_LIBRARY 
    NAMES boost_${name}
          boost_${name}-${Boost_LIB_SUFFIX}
          boost_${name}-${Boost_LIB_SUFFIX}-${Boost_VER}
    PATHS ${Boost_ROOT_DIR}/lib
          ${Boost_ROOT_DIR}/stage/lib
	      /usr/local/lib
	      /usr/lib
	)

ENDMACRO(BOOST_USE)

#SET(Boost_VER 1_31)
#SET(Boost_INCLUDE_DIR ${Boost_ROOT_DIR}/include/boost-1_32)
	    
IF(NOT HAVE_BOOST_LIBRARIES)
	EXEC_PROGRAM(${CMAKE_C_COMPILER}
    ARGS -E \"${CMAKE_SOURCE_DIR}/cmake/CheckBoostVersion.cpp\"
	OUTPUT_VARIABLE CMAKE_COMPILER_OUTPUT
  	RETURN_VALUE CMAKE_COMPILER_RETURN
	)

	STRING(REGEX MATCH "version=\"[0-9_]+\"" OUTP "${CMAKE_COMPILER_OUTPUT}")
	STRING(REGEX MATCH "[0-9_]+" BOOST_VERSION "${OUTP}")
	SET(HAVE_BOOST_LIBRARIES 1)
	# MESSAGE("Boost version ${BOOST_VERSION}")
ENDIF(NOT HAVE_BOOST_LIBRARIES)

  