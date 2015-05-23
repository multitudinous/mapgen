#include "geotext.h"
#include "gldraw.h"

//============================================================================
//============================================================================
GeoText::GeoText() :
    _alignH(Define::align_h_ctr),
    _alignV(Define::align_v_ctr)
{
    
}

//============================================================================
//============================================================================
GeoText::GeoText(const GeoText &text) :
    GeoObj(text)
{
    _text = text.text();
    _ext = text.extents();
    _alignH = text.alignH();
    _alignV = text.alignV();
}

//============================================================================
//============================================================================
GlObj* GeoText::clone() const
{
    return new GeoText(*this);
}

//============================================================================
//============================================================================
void GeoText::text(const std::string &t)
{
    _text = t;
}

//============================================================================
//============================================================================
const std::string& GeoText::text() const
{
    return _text;
}

//============================================================================
//============================================================================
void GeoText::extents(const Extents &e)
{
    _ext = e;
}

//============================================================================
//============================================================================
const Extents& GeoText::extents() const
{
    return _ext;
}

//============================================================================
//============================================================================
void GeoText::alignH(Define::AlignH a)
{
    _alignH = a;
}

//============================================================================
//============================================================================
Define::AlignH GeoText::alignH() const
{
    return _alignH;
}

//============================================================================
//============================================================================
void GeoText::alignV(Define::AlignV a)
{
    _alignV = a;
}

//============================================================================
//============================================================================
Define::AlignV GeoText::alignV() const
{
    return _alignV;
}

//============================================================================
//============================================================================
void GeoText::draw(DrawData *pdd)
{
    GlObj::draw(pdd);

    if (!pdd) return;
    if (_text.size() == 0) return;
    if (_ext.isZero()) return;

    GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
    if (!pfont) return;

    box3d box = pfont->getBBoxd(_text.c_str(), _ext, _alignH, _alignV);

    drawFill(pdd, box);
    drawOutline(pdd, box);
    drawLabel(pdd, box);
}

//============================================================================
//============================================================================
void GeoText::drawFill(DrawData *pdd, const box3d &txbox)
{
    if (!DrawAttr::drawPolyFill(pdd->_drawAttr, _drawAttr)) return;
    Rgbf color = DrawAttr::colorPolyFill(pdd->_drawAttr, _drawAttr);

    glColor4fv(color.m_af);
    Gldraw::drawQuad(txbox.getLeft(), txbox.getRight(), txbox.getTop(), txbox.getBtm());
}

//============================================================================
//============================================================================
void GeoText::drawOutline(DrawData *pdd, const box3d &txbox)
{
    if (!DrawAttr::drawPolyOutline(pdd->_drawAttr, _drawAttr)) return;
    Rgbf color = DrawAttr::colorPolyOutline(pdd->_drawAttr, _drawAttr);

    glColor4fv(color.m_af);
    glLineWidth(1); // TODO: have line with as part of outline attribute
    Gldraw::drawQuadLine(txbox.getLeft(), txbox.getRight(), txbox.getTop(), txbox.getBtm());
}

//============================================================================
//============================================================================
void GeoText::drawLabel(DrawData *pdd, const box3d &txbox)
{
    GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
    if (!pfont) return;

    Rgbf color = DrawAttr::colorLabels(pdd->_drawAttr, _drawAttr);

    glColor4fv(color.m_af);
    pfont->renderBL(_text.c_str(), txbox.getBtmLeft().get2d());
}

//============================================================================
//============================================================================
void GeoText::computeBox(DrawData *pdd, box3d *pbox)
{
    GeoObj::computeBox(pdd, pbox);

    if (!pdd) return;

    if (_ext.isZero()) return;

    if (!DrawAttr::drawLabels(pdd->_drawAttr, _drawAttr)) return;
    std::string *pfeature = DrawAttr::feature(pdd->_drawAttr, _drawAttr);
    if (!pfeature) return;

    GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
    if (!pfont) return;

    if (_text.size() == 0) return;

    box3d txbox = pfont->getBBoxd(_text.c_str(), _ext, _alignH, _alignV);
    pbox->updateBox(txbox);

}

//============================================================================
//============================================================================
Point2d GeoText::getTextSize(DrawData *pdd)
{
    GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
    if (!pfont) return Point2d(0, 0);

    if (_text.size() == 0) return Point2d(0, 0);
    box3d box = pfont->getBBoxd(_text.c_str());

    return Point2d(box.getWidth(), box.getHeight());
}