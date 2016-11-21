#include "drawdata.h"
#include "utlglline.h"
#include "platform.h"
#include "utlqt.h"

//============================================================================
//============================================================================
DrawData::DrawData() :
    _winW(0),
    _winH(0),
    _cfg(new Config()),
    m_hGluTess(NULL),
    _filePaths(new FilePaths()),
    _movesys(new MoveSys(20.0f)),
    _ds(new DataSel()),
    _dm(new DataMouse()),
    _dv(new DataValidate()),
    m_shadersOn(true),
    m_wireframe(false),
    m_lightingOn(false),
    _drawSelected(false),
	_maskOutInner(false)
{

    //m_frustum.reset(new Frustum());
}

//============================================================================
//============================================================================
void DrawData::filePaths(PFilePaths fp)
{
    _filePaths = fp;
}

//============================================================================
//============================================================================
FilePaths* DrawData::filePaths()
{
    return _filePaths.get();
}

//============================================================================
//============================================================================
DataSel* DrawData::dataSel() 
{ 
    return _ds.get(); 
}

//============================================================================
//============================================================================
DataMouse* DrawData::dataMouse() 
{ 
    return _dm.get(); 
}

//============================================================================
//============================================================================
DataValidate* DrawData::dataValidate()
{
    return _dv.get();
}

//============================================================================
//============================================================================
bool DrawData::init()
{
    m_hGluTess = gluNewTess();
    _progSmoothLine = UtlGlLine::initSmoothShader();
    if (!_progSmoothLine) return false;

    return initOverlay();
}


//============================================================================
//============================================================================
bool DrawData::initOverlay()
{
    _drawAttrOverlay.reset();

    // init overlay font
    std::vector<std::string> searchPathFonts;
    Platform::getFontSearchPaths(&searchPathFonts);


    std::string fontfile;
    if (!UtlQt::findFile("ARIAL.TTF", searchPathFonts, &fontfile))
    {
        LogError("DrawData::initOverlay() - failed to find Arial font file!");
        return false;
    }

    _drawAttrOverlay.reset(new DrawAttr());
    _drawAttrOverlay->_font.initFontTexture(fontfile.c_str(), 16);
    _drawAttrOverlay->_colorLabels.SetColor(0, 0, 0, 1);

    return true;
}