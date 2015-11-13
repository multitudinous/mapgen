#ifndef GEOTEXT_H
#define GEOTEXT_H

#include "geoobj.h"
#include "extents.h"
#include "define.h"

class GeoText : public GeoObj
{
public:
    GeoText(bool multiline=false);
    GeoText(const GeoText &text);

    void text(const std::string &t);
    const std::string& text() const;

    void extents(const Extents &e);
    const Extents& extents() const;

    void alignH(Define::AlignH a);
    Define::AlignH alignH() const;

    void alignV(Define::AlignV a);
    Define::AlignV alignV() const;

    virtual void draw(DrawData *pdd);
    virtual void computeBox(DrawData *pdd, box3d *pbox);

    Point2d getTextSize(DrawData *pdd);

    virtual GlObj* clone() const;


protected:
    void drawFill(DrawData *pdd, const box3d &txbox);
    void drawOutline(DrawData *pdd, const box3d &txbox);
    void drawLabel(DrawData *pdd, const box3d &txbox);

    Point2d getTextSizeLayout(GlText *pfont);

protected:
    std::string _text;
    Extents _ext;
    Define::AlignH _alignH;
    Define::AlignV _alignV;
    bool _multiline;

};

#endif