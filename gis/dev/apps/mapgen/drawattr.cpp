#include "drawattr.h"

DrawAttr::DrawAttr()
{
    _drawName = false;
    _drawLabels = false;
    _feature = "";

    _useLabelPosOffset = false;
    _labelOfs.dX = 0;
    _labelOfs.dY = 0;

    _drawPolyFill = false;
    _drawPolyOutline = false;
    _drawPolyMasked = false;

    _alpha = 1.0;

    for (int i=0; i<4; i++)
    {
        _colorPolyFill[i] = 1;
        _colorPolyOutline[i] = 1;
        _colorLabels[i] = 1;
    }

    _overrideAlpha = false;
    _overrideDrawPolyFill = false;
    _overrideDrawPolyOutline = false;
    _overrideDrawPolyMasked = false;
    _overrideDrawLabels = false;
    _overrideColorPolyFill = false;
    _overrideColorPolyOutline = false;
    _overridePolyMask = false;
    _overrideColorLabels = false;
    _overrideFeature = false;
    _overrideFont = false;
}

//============================================================================
//============================================================================
bool DrawAttr::drawPolyFill(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::drawPolyFill(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
bool DrawAttr::drawPolyOutline(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::drawPolyOutline(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
bool DrawAttr::drawPolyMasked(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::drawPolyMasked(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
bool DrawAttr::drawLabels(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::drawLabels(plyr.get(), pover.get());
}

//============================================================================
// TODO: need alpha for each type, mask, fill, outline, label
//============================================================================
float DrawAttr::alpha(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::alpha(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
Rgbf DrawAttr::colorPolyFill(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::colorPolyFill(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
Rgbf DrawAttr::colorPolyOutline(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::colorPolyOutline(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
Rgbf DrawAttr::colorLabels(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::colorLabels(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
const vector< shared_ptr<GlObj> >* DrawAttr::maskPoly(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::maskPoly(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
GlText* DrawAttr::font(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::font(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
std::string* DrawAttr::feature(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover)
{
    return DrawAttr::feature(plyr.get(), pover.get());
}

//============================================================================
//============================================================================
bool DrawAttr::drawPolyFill(DrawAttr *plyr, DrawAttr *pover)
{
   if (pover && pover->_overrideDrawPolyFill) return true;
   if (plyr && plyr->_drawPolyFill) return true;

   return false;
}

//============================================================================
//============================================================================
bool DrawAttr::drawPolyOutline(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideDrawPolyOutline) return true;
    if (plyr && plyr->_drawPolyOutline) return true;

    return false;
}

//============================================================================
//============================================================================
bool DrawAttr::drawPolyMasked(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideDrawPolyMasked) return true;
    if (plyr && plyr->_drawPolyMasked) return true;

    return false;
}

//============================================================================
//============================================================================
bool DrawAttr::drawLabels(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideDrawLabels) return true;
    if (plyr && plyr->_drawLabels) return true;

    return false;
}

//============================================================================
//============================================================================
float DrawAttr::alpha(DrawAttr *plyr, DrawAttr *pover)
{
   if (pover && pover->_overrideAlpha) return pover->_alpha;
   if (plyr && plyr->_drawPolyFill) return plyr->_alpha;

   return 1.0;
}

//============================================================================
//============================================================================
Rgbf DrawAttr::colorPolyFill(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideColorPolyFill) return pover->_colorPolyFill;
    if (plyr) return plyr->_colorPolyFill;

    return Rgbf();
}

//============================================================================
//============================================================================
Rgbf DrawAttr::colorPolyOutline(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideColorPolyOutline) return pover->_colorPolyOutline;
    if (plyr) return plyr->_colorPolyOutline;

    return Rgbf();
}

//============================================================================
//============================================================================
Rgbf DrawAttr::colorLabels(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideColorLabels) return pover->_colorLabels;
    if (plyr) return plyr->_colorLabels;

    return Rgbf();
}

//============================================================================
//============================================================================
const vector< shared_ptr<GlObj> >* DrawAttr::maskPoly(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overridePolyMask) return &pover->_maskPolyList;
    if (plyr) return &plyr->_maskPolyList;

    return NULL;
}

//============================================================================
//============================================================================
GlText* DrawAttr::font(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideFont) return &pover->_font;
    if (plyr) return &plyr->_font;

    return NULL;
}


//============================================================================
//============================================================================
std::string* DrawAttr::feature(DrawAttr *plyr, DrawAttr *pover)
{
    if (pover && pover->_overrideFeature) return &pover->_feature;
    if (plyr) return &plyr->_feature;

    return NULL;
}
