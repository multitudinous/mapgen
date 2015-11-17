#ifndef GEOLAYER_H
#define GEOLAYER_H

#include "geoobj.h"

class GeoLayer : public GeoObj
{
public:
    GeoLayer();

    virtual void draw(DrawData *pdd);
    virtual void computeBox(DrawData *pdd, box3d *pbox);
    virtual void initAttr(DrawData *pdd);
    virtual void initLabels(DrawData *pdd);

    int renderFlags() const { return _renderFlags; }
    void  renderFlags(int flags) { _renderFlags = flags; }

    std::string exportColorMapFile() const  { return _exportColorMapFile;  }
    void exportColorMapFile(std::string exportFile) { _exportColorMapFile = exportFile; }

protected:
    void exportColorMap(DrawData *pdd);

protected:
    int _renderFlags;
    std::string _exportColorMapFile;
};

#endif
