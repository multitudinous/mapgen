#include "geoaerial.h"


//============================================================================
//============================================================================
GeoAerial::GeoAerial()
{
    _extExtra = 0;
}

//============================================================================
//============================================================================
GeoAerial::GeoAerial(PTexture tx, const Extents &ext) :
    GeoImg(tx, ext)
{
    _extExtra = 0;
}
