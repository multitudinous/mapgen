#-------------------------------------------------
#
# Project created by QtCreator 2014-06-02T00:39:50
#
#-------------------------------------------------

QT       += core gui network

TARGET = mapgen
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += _CRT_SECURE_NO_WARNINGS


SOURCES += main.cpp \
    utltxgeo.cpp \
    utlstring.cpp \
    utlmath.cpp \
    utlgl.cpp \
    utl3d.cpp \
    textures.cpp \
    texture.cpp \
    movesys.cpp \
    membuf.cpp \
    logger.cpp \
    glutsys.cpp \
    gltext.cpp \
    gldraw.cpp \
    gissys.cpp \
    gdalFile.cpp \
    filepaths.cpp \
    CDefine.cpp \
    camera.cpp \
    bingimg.cpp \
    gradientpicker.cpp \
    geojson.cpp \
    globj.cpp \
    geoobj.cpp \
    geopoly.cpp \
    geolayer.cpp \
    geocompute.cpp \
    drawdata.cpp \
    drawattr.cpp \
    utlline2d.cpp \
    point2d.cpp \
    utlmisc.cpp \
    rgbf.cpp \
    geoimg.cpp \
    fbo.cpp \
    utlqt.cpp \
    mapyaml.cpp \
    yaml/tag.cpp \
    yaml/stream.cpp \
    yaml/singledocparser.cpp \
    yaml/simplekey.cpp \
    yaml/scantoken.cpp \
    yaml/scantag.cpp \
    yaml/scanscalar.cpp \
    yaml/scanner.cpp \
    yaml/regex.cpp \
    yaml/parser.cpp \
    yaml/parse.cpp \
    yaml/ostream_wrapper.cpp \
    yaml/null.cpp \
    yaml/nodeevents.cpp \
    yaml/nodebuilder.cpp \
    yaml/node_data.cpp \
    yaml/node.cpp \
    yaml/memory.cpp \
    yaml/exp.cpp \
    yaml/emitterutils.cpp \
    yaml/emitterstate.cpp \
    yaml/emitter.cpp \
    yaml/emitfromevents.cpp \
    yaml/emit.cpp \
    yaml/directives.cpp \
    yaml/convert.cpp \
    yaml/binary.cpp \
    geoaerial.cpp \
    geowords.cpp \
    utlgeom2d.cpp \
    config.cpp \
    xml/utlXml.cpp \
    xml/tinyxmlparser.cpp \
    xml/tinyxmlerror.cpp \
    xml/tinyxml.cpp \
    xml/tinystr.cpp \
    mapxml.cpp \
    mapinput.cpp \
    batchcfgwordfind.cpp

HEADERS += \
    vector3.h \
    utltxgeo.h \
    utlstring.h \
    utlmath.h \
    utlgl.h \
    utl3d.h \
    textures.h \
    texture.h \
    sysdefs.h \
    movesys.h \
    membuf.h \
    matrix4x4.h \
    logger.h \
    glutsys.h \
    gltext.h \
    gldraw.h \
    gissys.h \
    gdalFile.h \
    filepaths.h \
    extents.h \
    CDefine.h \
    camera.h \
    basevector.h \
    bingimg.h \
    gradientpicker.h \
    geojson.h \
    globj.h \
    geoobj.h \
    geopoly.h \
    geolayer.h \
    geocompute.h \
    drawdata.h \
    drawattr.h \
    utlline2d.h \
    point2d.h \
    utlmisc.h \
    rgbf.h \
    geoimg.h \
    sleepsim.h \
    fbo.h \
    utlqt.h \
    mapyaml.h \
    yaml/yaml.h \
    yaml/traits.h \
    yaml/token.h \
    yaml/tag.h \
    yaml/stringsource.h \
    yaml/streamcharsource.h \
    yaml/stream.h \
    yaml/stlemitter.h \
    yaml/singledocparser.h \
    yaml/setting.h \
    yaml/scantag.h \
    yaml/scanscalar.h \
    yaml/scanner.h \
    yaml/regeximpl.h \
    yaml/regex.h \
    yaml/ptr_vector.h \
    yaml/ptr_stack.h \
    yaml/parser.h \
    yaml/ostream_wrapper.h \
    yaml/null.h \
    yaml/noncopyable.h \
    yaml/nodeevents.h \
    yaml/nodebuilder.h \
    yaml/mark.h \
    yaml/indentation.h \
    yaml/exp.h \
    yaml/exceptions.h \
    yaml/eventhandler.h \
    yaml/emitterutils.h \
    yaml/emitterstate.h \
    yaml/emittermanip.h \
    yaml/emitterdef.h \
    yaml/emitter.h \
    yaml/emitfromevents.h \
    yaml/dll.h \
    yaml/directives.h \
    yaml/collectionstack.h \
    yaml/binary.h \
    yaml/anchor.h \
    geoaerial.h \
    geowords.h \
    utlgeom2d.h \
    box3.h \
    box2.h \
    linkedlist.h \
    config.h \
    xml/utlXml.h \
    xml/tinyxml.h \
    xml/tinystr.h \
    mapxml.h \
    mapinput.h \
    batchcfg.h \
    batchcfgwordfind.h

symbian: LIBS += -lftgl_D
else:unix|win32: LIBS += -L$$PWD/../../libs/ftgl/x64/lib/ -lftgl_D

INCLUDEPATH += $$PWD/../../libs/ftgl/x64/include
DEPENDPATH += $$PWD/../../libs/ftgl/x64/include

symbian: LIBS += -lglew32
else:unix|win32: LIBS += -L$$PWD/../../libs/glew-1.10.0/lib/Release/x64/ -lglew32

INCLUDEPATH += $$PWD/../../libs/glew-1.10.0/include/GL
DEPENDPATH += $$PWD/../../libs/glew-1.10.0/include/GL

symbian: LIBS += -lgdal_i
else:unix|win32: LIBS += -L$$PWD/../../libs/gdal/x64/lib/ -lgdal_i

INCLUDEPATH += $$PWD/../../libs/gdal/x64/include
DEPENDPATH += $$PWD/../../libs/gdal/x64/include

symbian: LIBS += -lfreeglut
else:unix|win32: LIBS += -L$$PWD/../../libs/freeglut/lib/x64/ -lfreeglut

INCLUDEPATH += $$PWD/../../libs/freeglut/include/GL
DEPENDPATH += $$PWD/../../libs/freeglut/include/GL

symbian: LIBS += -lfreetype
else:unix|win32: LIBS += -L$$PWD/../../libs/freetype/x64/lib/ -lfreetype

INCLUDEPATH += $$PWD/../../libs/freetype/x64/include
DEPENDPATH += $$PWD/../../libs/freetype/x64/include

INCLUDEPATH += $$PWD/../../../../../d/boost_1_35_0
INCLUDEPATH += $$PWD/yaml/include
