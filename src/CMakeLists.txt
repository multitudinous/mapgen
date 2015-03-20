
file( GLOB HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${BOOST_INCLUDE_DIR} )
include_directories( ${YAMLCPP_INCLUDE_DIRS} )
include_directories( ${GDAL_INCLUDE_DIR} )
include_directories( ${GLEW_INCLUDE_DIR} )
include_directories( ${GLEW_INCLUDE_DIR}/GL )
include_directories( ${FREEGLUT_INCLUDE_DIR} )
include_directories( ${FTGL_INCLUDE_DIR} )
include_directories( ${FREETYPE_INCLUDE_DIRS} )
#include_directories( ${Boost_INCLUDE_DIRS} )

#add_definitions( -DLEAF_GIS_LIBRARY )
#add_definitions( -D_USE_MATH_DEFINES )

IF(APPLE)
   INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIRS} )
ENDIF (APPLE)

#set( HDRS_TO_MOC
#    bingimg.h
#    )
#QT4_WRAP_CPP( MOCCED ${HDRS_TO_MOC} )

set( TARGET_NAME mapgen )
set( TARGET_CATEGORY App )
set( TARGET_LANGUAGE CXX )
set( TARGET_VERSION ${VES_PACKAGE_VERSION} )
set( INSTALL_BINDIR ${VES_INSTALL_BINDIR} )

add_executable( ${TARGET_NAME} ${HEADERS} ${SOURCES} ${MOCCED})
target_link_libraries( ${TARGET_NAME}
    ${YAMLCPP_LIBRARIES}
    ${GDAL_LIBRARY}
    ${OPENGL_LIBRARIES}
	${FREEGLUT_LIBRARY}
	${GLEW_LIBRARY}
    ${FTGL_LIBRARY}
    ${FREETYPE_LIBRARIES}
    Qt5::Gui
    Qt5::Network
    )
    

include( VESModuleInstall )
