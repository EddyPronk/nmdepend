FIND_PATH(Binutils_ROOT_DIR include/bfd.h
	$ENV{Binutils_ROOT_DIR})

SET(Binutils_INCLUDE_DIR ${Binutils_ROOT_DIR}/include)

FIND_LIBRARY(BFD_LIBRARY 
  NAMES bfd 
  PATHS /usr/lib ${Binutils_ROOT_DIR}/bfd
        /usr/$ENV{MACHTYPE}/lib
  DOC "Regular expressions. Boost >=1.31 has compiler suffix for library names")

#FIND_LIBRARY(INTL_LIBRARY 
#  NAMES intl 
#  PATHS /usr/lib ${Binutils_ROOT_DIR}/intl
#  DOC "Regular expressions. Boost >=1.31 has compiler suffix for library names")

FIND_LIBRARY(IBERTY_LIBRARY
  NAMES iberty 
  PATHS /usr/lib ${Binutils_ROOT_DIR}/lib/x86_64
        /usr/$ENV{MACHTYPE}/lib
  DOC "Regular expressions. Boost >=1.31 has compiler suffix for library names")

