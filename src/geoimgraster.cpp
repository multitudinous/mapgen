#include "geoimgraster.h"

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster()
{

}

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster(PTexture tx, const Extents &ext) : 
GeoImg(tx, ext)
{

}

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster(PTexture tx, const Extents &ext, PGradientPicker picker, const Stats &stats) :
GeoImg(tx, ext),
_picker(picker),
_stats(stats)

{

}