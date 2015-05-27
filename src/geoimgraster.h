#ifndef GEOIMGRASTER_H
#define GEOIMGRASTER_H

#include "geoimg.h"
#include "gradientpicker.h"
#include "stats.h"

class GeoImgRaster : public GeoImg
{
public:
    GeoImgRaster();
    GeoImgRaster(PTexture tx, const Extents &ext);
    GeoImgRaster(PTexture tx, const Extents &ext, PGradientPicker picker, const Stats &stats);

    void stats(const Stats &s) { _stats = s; }
    const Stats& stats() const { return _stats;  }

    void picker(PGradientPicker picker) { _picker = picker;  }
    const GradientPicker* picker() const { return _picker.get(); }

protected:
    Stats _stats;
    PGradientPicker _picker;
};

#endif