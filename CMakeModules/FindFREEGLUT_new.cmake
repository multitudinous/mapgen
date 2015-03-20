# - Find a freeglut installation or build tree.
# The following variables are set if freeglut is found.  If freeglut is not
# found, freeglut_FOUND is set to false.
#  freeglut_FOUND         - Set to true when freeglut is found.
#  freeglut_USE_FILE      - CMake file to use freeglut.
#  freeglut_MAJOR_VERSION - The freeglut major version number.
#  freeglut_MINOR_VERSION - The freeglut minor version number 
#                       (odd non-release).
#  freeglut_BUILD_VERSION - The freeglut patch level 
#                       (meaningless for odd minor).
#  freeglut_INCLUDE_DIRS  - Include directories for freeglut
#  freeglut_LIBRARY_DIRS  - Link directories for freeglut libraries

# The following cache entries must be set by the user to locate freeglut:
#  freeglut_DIR  - The directory containing freeglutConfig.cmake.  
#             This is either the root of the build tree,
#             or the lib directory.  This is the 
#             only cache entry.


# Assume not found.
SET(freeglut_FOUND 0)

# Construct consitent error messages for use below.
SET(freeglut_DIR_DESCRIPTION "directory containing freeglutConfig.cmake.  This is either the root of the build tree, or PREFIX/lib for an installation.")
SET(freeglut_DIR_MESSAGE "freeglut not found.  Set the freeglut_DIR cmake cache entry to the ${freeglut_DIR_DESCRIPTION}")

# Use the Config mode of the find_package() command to find freeglutConfig.
# If this succeeds (possibly because freeglut_DIR is already set), the
# command will have already loaded freeglutConfig.cmake and set freeglut_FOUND.
IF(NOT freeglut_FOUND)
  FIND_PACKAGE(freeglut QUIET NO_MODULE)
ENDIF(NOT freeglut_FOUND)

#-----------------------------------------------------------------------------
IF(NOT freeglut_FOUND)
  # freeglut not found, explain to the user how to specify its location.
  IF(freeglut_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR ${freeglut_DIR_MESSAGE})
  ELSE(freeglut_FIND_REQUIRED)
    IF(NOT freeglut_FIND_QUIETLY)
      MESSAGE(STATUS ${freeglut_DIR_MESSAGE})
    ENDIF(NOT freeglut_FIND_QUIETLY)
  ENDIF(freeglut_FIND_REQUIRED)
ENDIF(NOT freeglut_FOUND)

