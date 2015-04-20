#ifndef GEOIMG_H
#define GEOIMG_H

#include "geoobj.h"
#include "texture.h"
#include "extents.h"

class GeoImg : public GeoObj
{
public:
    GeoImg();
    GeoImg(PTexture tx, const Extents &ext);

    virtual void draw(DrawData *pdd);
    virtual void computeBox(DrawData *pdd, box3d *pbox);

    void init(PTexture tx, const Extents &ext) { _tx = tx; _ext = ext; }

   protected:
        void drawImg(float alpha=1.0f);

protected:
    Extents _ext;
    PTexture _tx;
};

#endif
