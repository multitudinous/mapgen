#ifndef UTLTXGEO_H
#define UTLTXGEO_H

#include "texture.h"
#include "gdalFile.h"

class QImage;
class GradientPicker;

class UtlTxGeo
{
public:
	UtlTxGeo(){};

    static PTexture load(const QImage *img);

    static PTexture loadHmap(const GdalFile *pfile, GradientPicker *pcolor = NULL, Stats *statSettings = NULL);
	static PTexture loadRgb(const GdalFile *pfile);

    static PMemBuf hmapToRgb(const MemBuf *hmap, const Stats &stats, GradientPicker *pcolor=NULL);
};

#endif
