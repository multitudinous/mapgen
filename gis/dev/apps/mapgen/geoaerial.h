#ifndef GEOAERIAL_H
#define GEOAERIAL_H

#include "geoimg.h"

class GeoAerial : public GeoImg
{
public:
    GeoAerial();
    GeoAerial(PTexture tx, const Extents &ext);

public:
    double _extExtra;

};

#endif
