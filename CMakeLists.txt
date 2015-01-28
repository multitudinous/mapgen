cmake_minimum_required (VERSION 2.8.8)

project( mapgen )

SET( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules" )

set(mapgen_MAJOR_VERSION 0)
set(mapgen_MINOR_VERSION 1)
set(mapgen_SUB_VERSION 0)
set(mapgen_VERSION ${mapgen_MAJOR_VERSION}.${mapgen_MINOR_VERSION}.${mapgen_SUB_VERSION})
set(VES_PACKAGE_VERSION ${mapgen_VERSION})

#Set the include directory so that mapgen can be used as a sub project
if( NOT(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME}) )
    set(mapgen_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src PARENT_SCOPE)
    set(mapgen_LIBRARIES "mapgen" PARENT_SCOPE)
endif()

find_package( GDAL )
find_package( yamlcpp )
find_package( FTGL )

FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore QtGui )
include(${QT_USE_FILE})

include(VESSetupCompiler)

include(VESBinAndLibDirs)

add_subdirectory(src)

#include(VESInstallPDBFiles)

set(VES_EXPORT_LIBRARY_TARGETS mapgen)
set(VES_CONFIGURE_VARS CMAKE_INSTALL_LIBDIR)

#
# Setup the fpc auto detection files

#include(VESInstallAutoDiscoveryTools)
