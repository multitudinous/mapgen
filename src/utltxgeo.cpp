#include "utltxgeo.h"
#include "colorpicker.h"
#include <QImage>

//============================================================================
//============================================================================
PTexture UtlTxGeo::load(const QImage *img)
{
    int bytesPerPix = img->depth() / 8;
    if (bytesPerPix != 3 && bytesPerPix != 4)
    {
        return PTexture();
    }

    PMemBuf rgb(new MemBuf());
    rgb->setSize(bytesPerPix, img->width(), img->height());
    unsigned char *dst = (unsigned char *)rgb->getBuf();

    const unsigned char *bytes = img->bits();
    for (int y=0; y<img->height(); y++)
    {
        const unsigned char *src = bytes + img->bytesPerLine() * y;
        for (int x=0; x<img->width(); x++)
        {
            for (int i=0; i<bytesPerPix; i++)
            {
                *dst = *src;
                dst++;
                src++;
            }
        }
    }


    PTexture tx(new Texture());
    tx->create(rgb, GL_CLAMP_TO_EDGE);

    return tx;
}

//============================================================================
//============================================================================
PTexture UtlTxGeo::loadHmap(const GdalFile *pfile, ColorPicker *pcolor, Stats *statSettings, bool validation, PMemBuf *phmap)
{
	PMemBuf hmap(new MemBuf());
    Stats stats;
	Stats *s = &stats;
    if (statSettings) s = statSettings;

    if (phmap) *phmap = hmap;

	if (!pfile->GetHmap(hmap.get(), s))
	{
		return PTexture();
	}

    s->computeAdjusted();

    PMemBuf rgb = hmapToRgb(hmap.get(), *s, pcolor, validation);

	PTexture tx(new Texture());
    tx->create(rgb, GL_CLAMP_TO_EDGE, Texture::I_FILTER_NONE);

	return tx;
}

//============================================================================
//============================================================================
PTexture UtlTxGeo::loadRgb(const GdalFile *pfile)
{
	PMemBuf rgb(new MemBuf());
	if (!pfile->GetRGB(rgb.get()))
	{
		return PTexture();
	}

	PTexture tx(new Texture());
	tx->create(rgb, GL_CLAMP_TO_EDGE);
	return tx;
}

//============================================================================
//============================================================================
PMemBuf UtlTxGeo::hmapToRgb(const MemBuf *hmap, const Stats &stats, ColorPicker *pcolor, bool validation)
{
    PMemBuf rgb(new MemBuf(4, hmap->getLenX(), hmap->getLenY()));

	const double *pbuf = (const double *)hmap->getBufRead();
	BYTE *prgb = (BYTE *)rgb->getBuf();
    double dis = stats.getLen();

    BYTE rgbaOn[4];
    BYTE rgbaOf[4];
    bool useonof = false;

    // used for on off coloring and also nodata values
    rgbaOf[0] = 0;
    rgbaOf[1] = 0;
    rgbaOf[2] = 0;
    rgbaOf[3] = 0;

    // on or off coloring
    if (fabs(dis) <= .0001)
    {
        QColor c = pcolor->pickByPercent(1);
        rgbaOn[0] = (BYTE)c.red();
        rgbaOn[1] = (BYTE)c.green();
        rgbaOn[2] = (BYTE)c.blue();
        rgbaOn[3] = 255;

        useonof = true;
    }

	//FILE *fp = fopen("debug.txt", "w");
	//fprintf(fp, "min: %.2f, max: %.2f, mean: %.2f, stddev: %.2f\n", stats.min, stats.max, stats.mean, stats.stddev);
	for (int y=0; y<hmap->getLenY(); y++)
	{
		for (int x=0; x<hmap->getLenX(); x++)
		{
			double d = *pbuf;
			pbuf++;

            // is this a no data value
            if (d == stats._nodataValue)
            {
                memcpy(prgb, rgbaOf, 4);
                prgb += 4;
                continue;
            }

            if (useonof)
            {
                if (d > .001)
                {
                    memcpy(prgb, rgbaOn, 4);
                }
                else
                {
                    memcpy(prgb, rgbaOf, 4);
                }

                prgb += 4;
                continue;
            }
		
            // get the normalized position
            double hn = stats.getPosNorm(d);

		
            /*
            // color between 2 std deviation
			double devs = 1.75;
			double div = 2.0*devs*stats.stddev; // must multiple by 2 because its devs positive and devs negative away from mean
			double tomean = d - stats.mean;
			double tomeanpos = tomean + devs*stats.stddev;
			double hn = tomeanpos / div;

			//fprintf(fp, "d: %.2f, tomean: %.2f, tomeanpos: %.2f, divisor: %.2f, norm: %.2f\n", d, tomean, tomeanpos, div, hn);

			if (hn < 0) hn = 0;
			if (hn > 1) hn = 1;
			//double hn = (d - stats.min) / dis;
            */

            if (pcolor)
            {
                QColor c = pcolor->pickPrefered(hn, d, validation);
                prgb[0] = (BYTE)c.red();
                prgb[1] = (BYTE)c.green();
                prgb[2] = (BYTE)c.blue();
                prgb[3] = 255;
                prgb += 4;
            }
            else
            {
                double c = 255.0 * hn;
                if (c > 255) c = 255;
                if (c < 0) c = 0;

                prgb[0] = (BYTE)c;
                prgb[1] = (BYTE)c;
                prgb[2] = (BYTE)c;
                prgb[3] = 255;
                prgb += 4;
            }

		}
	}

	//fclose(fp);
	return rgb;
}
