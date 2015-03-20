# - Locate BOOST library
# This module defines
#  BOOST_LIBRARIES, the library to link against
#  BOOST_FOUND, if false, do not try to link to BOOST
#  BOOST_INCLUDE_DIRS, where to find headers.
#
# $BOOST_DIR is an environment variable that points to the main ftgl directory.

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

FIND_PATH(BOOST_INCLUDE_DIR boost/shared_ptr.hpp
  HINTS
  $ENV{BOOST_DIR}
  PATH_SUFFIXES include src
  PATHS
  /usr/include
  /usr/local/include
  /sw/include
  /opt/local/include
  /usr/freeware/include
)

# set the user variables
IF(BOOST_INCLUDE_DIR)
  SET(BOOST_INCLUDE_DIRS "${BOOST_INCLUDE_DIR}")
ENDIF()

# handle the QUIETLY and REQUIRED arguments and set FREEGLUT_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE("FindPackageHandleStandardArgs")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BOOST DEFAULT_MSG  BOOST_INCLUDE_DIR)

MARK_AS_ADVANCED(BOOST_INCLUDE_DIR)

