#ifndef GEOIMGRASTER_H
#define GEOIMGRASTER_H

#include "geoimg.h"
#include "geotext.h"
#include "colorpicker.h"
#include "stats.h"

class GeoImgRaster : public GeoImg
{
public:
    GeoImgRaster();
    GeoImgRaster(PTexture tx, const Extents &ext);
    GeoImgRaster(PTexture tx, const Extents &ext, PColorPicker picker, const Stats &stats, PMemBuf dataValues);

    virtual void draw(DrawData *pdd);
    virtual void runSel(DrawData *pdd);

    void stats(const Stats &s) { _stats = s; }
    const Stats& stats() const { return _stats;  }

    void picker(PColorPicker picker) { _picker = picker;  }
    const ColorPicker* picker() const { return _picker.get(); }

protected:
    Stats _stats;
    PColorPicker _picker;
    PMemBuf _dataValues;

    std::vector<BYTE> _cselimg; // selected locations color in the texture
    std::vector<BYTE> _cselscr; // selected locations color on the screen
    double _seldatav; // selected locations data value
    double _seldatano; // selected locations normalized value
    double _seldatana; // selected locations normalized value adjusted
    int _selmx;
    int _selmy;
    GeoText _seltext;
};

#endif