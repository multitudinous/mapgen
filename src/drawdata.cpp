#include "drawdata.h"
#include "utlglline.h"

//============================================================================
//============================================================================
DrawData::DrawData() :
    _cfg(new Config()),
    m_hGluTess(NULL),
    m_shadersOn(true),
    m_wireframe(false),
    m_lightingOn(false)
{

    //m_frustum.reset(new Frustum());
}

//============================================================================
//============================================================================
bool DrawData::init()
{
    m_hGluTess = gluNewTess();
    _progSmoothLine = UtlGlLine::initSmoothShader();
    if (!_progSmoothLine) return false;

    return true;
}
