#include "geolayer.h"

//============================================================================
//============================================================================
GeoLayer::GeoLayer() :
    _msaaOn(true)
{
}

//============================================================================
//============================================================================
void GeoLayer::draw(DrawData *pdd)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    GlObj::draw(pdd);

    pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::computeBox(DrawData *pdd, box3d *pbox)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    GlObj::computeBox(pdd, pbox);

    pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::initAttr(DrawData *pdd)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    GlObj::initAttr(pdd);

    pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::initLabels(DrawData *pdd)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    GlObj::initLabels(pdd);

    pdd->_drawAttr = drawprev;
}
