#ifndef GEOOBJ_H
#define GEOOBJ_H

#include "globj.h"
#include "drawattr.h"

class GeoObj : public GlObj
{
public:
    GeoObj();
    GeoObj(const GeoObj &obj);

    virtual void setDrawAttr(PDrawAttr da) { _drawAttr = da; }
    PDrawAttr getDrawAttr() { return _drawAttr; }
    virtual void initAttr(DrawData *pdd);

    virtual bool haveFeature(DrawData *pdd, std::string *value);
    virtual bool getFeatureValue(const std::string &feature, double *value);
    virtual bool getFeatureValue(const std::string &feature, std::string *value);

    virtual GlObj* clone() const;

protected:
    PDrawAttr _drawAttr;


};

#endif
