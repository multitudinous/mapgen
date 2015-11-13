#ifndef UTLTXGEO_H
#define UTLTXGEO_H

#include "texture.h"
#include "gdalFile.h"

class QImage;
class ColorPicker;

class UtlTxGeo
{
public:
	UtlTxGeo(){};

    static PTexture load(const QImage *img);

    static PTexture loadHmap(const GdalFile *pfile, ColorPicker *pcolor = NULL, Stats *statSettings = NULL, bool validation = false, PMemBuf *phmapData=NULL);
	static PTexture loadRgb(const GdalFile *pfile);

    static PMemBuf hmapToRgb(const MemBuf *hmap, const Stats &stats, ColorPicker *pcolor = NULL, bool validation = false);
};

#endif
