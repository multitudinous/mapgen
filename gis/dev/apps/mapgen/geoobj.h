#ifndef GEOOBJ_H
#define GEOOBJ_H

#include "GlObj.h"
#include "drawattr.h"

class GeoObj : public GlObj
{
public:
    GeoObj();
    GeoObj(const GeoObj &obj);

    virtual void setDrawAttr(PDrawAttr da) { _drawAttr = da; }
    PDrawAttr getDrawAttr() { return _drawAttr; }

    virtual GlObj* clone() const;
protected:
    PDrawAttr _drawAttr;


};

#endif
