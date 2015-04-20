#ifndef GEOLAYER_H
#define GEOLAYER_H

#include "geoobj.h"

class GeoLayer : public GeoObj
{
public:
    GeoLayer();

    virtual void draw(DrawData *pdd);
    virtual void computeBox(DrawData *pdd, box3d *pbox);
    virtual void initLabels(DrawData *pdd);
};

#endif
