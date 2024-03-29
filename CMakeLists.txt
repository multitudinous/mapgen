cmake_minimum_required (VERSION 3.0.0)

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

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
#LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
#IF("${isSystemDir}" STREQUAL "-1")
#   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#ENDIF("${isSystemDir}" STREQUAL "-1")

find_package( GDAL REQUIRED )
find_package( yamlcpp REQUIRED )
find_package( FTGL REQUIRED )
find_package( Freetype REQUIRED )
find_package( GLEW REQUIRED )
find_package( GLUT REQUIRED )
find_package( OpenGL REQUIRED )
#find_package( PNG REQUIRED )
#find_package( ZLIB REQUIRED )

#Boost
if( WIN32 )
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()
set( BOOST_USE_STATIC_LIBS OFF )
set( BOOST_USE_MULTITHREADED ON )
set( BOOST_USE_STATIC_RUNTIME OFF )
set( BOOST_ROOT "Please specify"
    CACHE PATH "Root directory where Boost is installed" )
find_package( Boost 1.56.0 REQUIRED COMPONENTS )

# OLD QT4 CMake Code
# FIND_PACKAGE(Qt5 REQUIRED COMPONENTS QtCore QtGui QtNetwork)
# include(${QT_USE_FILE})

# QT5 CMake Code
# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Svg REQUIRED)


include(VESSetupCompiler)

include(VESBinAndLibDirs)

add_subdirectory(src)

#include(VESInstallPDBFiles)

set(VES_EXPORT_LIBRARY_TARGETS mapgen)
set(VES_CONFIGURE_VARS CMAKE_INSTALL_LIBDIR)

#qt5_use_modules(mapgen Core)
#qt5_use_modules(mapgen Gui)
#qt5_use_modules(mapgen Network)

#
# Setup the fpc auto detection files

#include(VESInstallAutoDiscoveryTools)
