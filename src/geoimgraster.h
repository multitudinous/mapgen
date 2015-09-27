#ifndef GEOIMGRASTER_H
#define GEOIMGRASTER_H

#include "geoimg.h"
#include "colorpicker.h"
#include "stats.h"

class GeoImgRaster : public GeoImg
{
public:
    GeoImgRaster();
    GeoImgRaster(PTexture tx, const Extents &ext);
    GeoImgRaster(PTexture tx, const Extents &ext, PColorPicker picker, const Stats &stats);

    void stats(const Stats &s) { _stats = s; }
    const Stats& stats() const { return _stats;  }

    void picker(PColorPicker picker) { _picker = picker;  }
    const ColorPicker* picker() const { return _picker.get(); }

protected:
    Stats _stats;
    PColorPicker _picker;
};

#endif