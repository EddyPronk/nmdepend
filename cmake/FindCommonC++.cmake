FIND_PATH(CommonC++_ROOT_DIR include/cc++2/cc++/config.h 
			${CommonCpp_ROOT_DIR})

SET(CommonC++_INCLUDE_DIR ${CommonC++_ROOT_DIR}/include/cc++2) 

FIND_LIBRARY(CommonC++_ccext2_LIBRARY
  ccext2
  ${CommonC++_ROOT_DIR}/lib
  /usr/lib
  /usr/local/lib
  /cygdrive/d/projects/libraries/export/lib  
  )

FIND_LIBRARY(CommonC++_ccgnu2_LIBRARY
  ccgnu2
  ${CommonC++_ROOT_DIR}/lib
  /usr/lib
  /usr/local/lib
  /cygdrive/d/projects/libraries/export/lib
  )

