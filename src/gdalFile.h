#ifndef GDALFILE_H
#define GDALFILE_H

#include "sysdefs.h"
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "extents.h"

//#pragma comment(lib, "gdal_i.lib") 
//#pragma comment(lib, "libcurl_imp.lib")

class MemBuf;


struct Stats
{
	double min;
	double max;
	double mean;
	double stddev;

	Stats()
	{
		min = 0;
		max = 0;
		mean = 0;
		stddev = 0;
	}
};

class GdalFile
{
public:
	GdalFile();
	virtual ~GdalFile();

	bool Load(const char *file);
	bool LoadBingWms(const Extents &ext);
	bool LoadBingTms(const Extents &ext, int tilelevel=5);
    bool WmsTest();

	bool saveToTiff(const char *path);

	bool GetChannelDataType(int channelid, GDALDataType *pType) const; // channel id start at one and go to including m_c
	bool GetChannelInfo(int channelid, std::string *pInfo) const;
	int GetChannelCount() const { return m_c; }

	bool GetRGB(MemBuf *pbuf, bool addAlpha=false) const;
	bool GetHmap(MemBuf *pbuf, Stats *pstats=NULL) const;
	bool ValidImage(int numChannels=0) const;

	Stats GetStats() { return m_stats; }
	const Extents* GetExtents() { return &m_extents; }
    //void GetExtentsUtm(UtmPos *pTL, UtmPos *pTR, UtmPos *pBL, UtmPos *pBR);
    //void GetExtentsUtm(UtmPos *pTL, UtmPos *pBR);
	int GetPixelW() { return m_w; }
	int GetPixelH() { return m_h; }

	static void Msg(std::string *pMsg, const char *frmt, ...);
protected:
	bool InitDataset();
	bool ValidChannel(int channelid) const;
	void Close();
	void logError(const char *func, const char *msg) const;

protected:
	GDALDataset  *m_pds;
	double       m_gt[6];	// geotransform
	int			 m_w;
	int		     m_h;
	int			 m_c;		// channels
	Extents		 m_extents;
	Stats		 m_stats;
	std::string m_name;
};

typedef shared_ptr<GdalFile> PGdalFile;

#endif
