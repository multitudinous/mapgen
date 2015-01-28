
file( GLOB HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" )
file( GLOB SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp" )

include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${YAMLCPP_INCLUDE_DIRS} )
include_directories( ${GDAL_INCLUDE_DIR} )
include_directories( ${GLEW_INCLUDE_DIR} )
include_directories( ${GLEW_INCLUDE_DIR}/GL )
include_directories( ${FTGL_INCLUDE_DIR} )
include_directories( ${FREETYPE_INCLUDE_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

#add_definitions( -DLEAF_GIS_LIBRARY )
#add_definitions( -D_USE_MATH_DEFINES )

#add_library( ${TARGET_NAME} SHARED ${SOURCES} ${HEADERS} )

IF(APPLE)
   INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIRS} )
   #SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)

set( TARGET_NAME mapgen )
set( TARGET_CATEGORY App )
set( TARGET_LANGUAGE CXX )
set( TARGET_VERSION ${VES_PACKAGE_VERSION} )
set( INSTALL_BINDIR ${VES_INSTALL_BINDIR} )

add_executable( ${TARGET_NAME} ${HEADERS} ${SOURCES} )
target_link_libraries( ${TARGET_NAME}
    ${YAMLCPP_LIBRARIES}
    ${GDAL_LIBRARY}
    ${GLEW_LIBRARY}
    ${GLUT_LIBRARY}
    ${FTGL_LIBRARY}
    ${FREETYPE_LIBRARIES}
    ${Boost_LIBRARIES}
    )

include( VESModuleInstall )
