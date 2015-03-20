# - Locate FREEGLUT library
# This module defines
#  FREEGLUT_LIBRARIES, the library to link against
#  FREEGLUT_FOUND, if false, do not try to link to FREEGLUT
#  FREEGLUT_INCLUDE_DIRS, where to find headers.
#
# $FREEGLUT_DIR is an environment variable that points to the main ftgl directory.

#=============================================================================
# Copyright 2011 Richard Ulrich.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

# Created by Richard Ulrich.

if (CMAKE_BUILD_TYPE MATCHES "debug")
        set(FREEGLUT_LIB_NAME freeglutd.lib)
    else()
        set(FREEGLUT_LIB_NAME freeglut.lib)
endif()

FIND_PATH(FREEGLUT_INCLUDE_DIR GL/freeglut.h 
  HINTS
  $ENV{FREEGLUT_DIR}
  PATH_SUFFIXES include src
  PATHS
  /usr/include
  /usr/local/include
  /sw/include
  /opt/local/include
  /usr/freeware/include
)

FIND_LIBRARY(FREEGLUT_LIBRARY
  NAMES freeglut
  HINTS
  $ENV{FREEGLUT_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  /usr/lib
  /usr/local/lib
  /sw
  /usr/freeware
)

# set the user variables
IF(FREEGLUT_INCLUDE_DIR)
  SET(FREEGLUT_INCLUDE_DIRS "${FREEGLUT_INCLUDE_DIR}")
ENDIF()

SET(FREEGLUT_LIBRARIES "${FREEGLUT_LIBRARY}")
#SET_TARGET_PROPERTIES(${FREEGLUT_LIBRARY} PROPERTIES DEBUG_POSTFIX "d")

# handle the QUIETLY and REQUIRED arguments and set FREEGLUT_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE("FindPackageHandleStandardArgs")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREEGLUT  DEFAULT_MSG  FREEGLUT_LIBRARY  FREEGLUT_INCLUDE_DIR)

MARK_AS_ADVANCED(FREEGLUT_LIBRARY FREEGLUT_INCLUDE_DIR)

