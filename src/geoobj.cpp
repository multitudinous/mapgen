#include "geoobj.h"

//============================================================================
//============================================================================
GeoObj::GeoObj()
{
}

//============================================================================
//============================================================================
GeoObj::GeoObj(const GeoObj &obj) :
    GlObj(obj)
{
    _drawAttr = obj._drawAttr;
}

//============================================================================
//============================================================================
void GeoObj::initAttr(DrawData *pdd)
{
    // no attribute then nothing to do
    if (!pdd->_drawAttr)
    {
        GlObj::initAttr(pdd);
        return;
    }

    // if we already have an override use it
    if (_drawAttr)
    {
        GlObj::initAttr(pdd);
        return;
    }

    // create an override for random colors
    if (pdd->_drawAttr->haveRandColor() || pdd->_drawAttr->haveColorByFeature())
    {
        PDrawAttr attr(new DrawAttr(*pdd->_drawAttr));
        std::string feature = "";

        // get poly fill color by feature
        if (pdd->_drawAttr->haveColorByFeaturePolyFill() && haveFeature(pdd, &feature))
        {
            attr->_colorPolyFill = pdd->_drawAttr->getColorPolyFill(feature);
            attr->_overrideColorPolyFill = true;
        }
        else if (pdd->_drawAttr->haveRandColorPolyFill()) // get random fill color
        { 
            attr->_colorPolyFill = pdd->_drawAttr->getColorPolyFill();
            attr->_overrideColorPolyFill = true;
        }

        if (pdd->_drawAttr->haveRandColorPolyOutline())
        {
            attr->_colorPolyOutline = pdd->_drawAttr->getColorPolyOutline();
            attr->_overrideColorPolyOutline = true;
        }

        if (pdd->_drawAttr->haveRandColorLabels())
        {
            attr->_colorLabels = pdd->_drawAttr->getColorLabels();
            attr->_overrideColorLabels = true;
        }

        setDrawAttr(attr);
    }
   
    GlObj::initAttr(pdd);
}

//============================================================================
//============================================================================
bool GeoObj::haveFeature(DrawData *pdd, std::string *value)
{
    return false;
}

//============================================================================
//============================================================================
GlObj* GeoObj::clone() const
{
    return new GeoObj(*this);
}
