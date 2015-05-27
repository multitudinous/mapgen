#include "gdalFile.h"
#include "membuf.h"
#include "logger.h"
#include "sysdefs.h"

///Desired output spatial reference as EPSG number.
#define EPSG_DESIRED_SPATIALREF    3857
///Minimum area of a polygon in EPSG_DESIRED_SPATIALREF units for generating a label.
#define MIN_AREA_LABEL .0001
///Pixel dimension of output map width and height
#define OUTPUT_PIXEL_SIZE 1200

#define TEMPFILE_BASEIMAGE "tempBaseImage.tif"

//============================================================================
//============================================================================
GdalFile::GdalFile()
{
	m_pds = NULL;
}

//============================================================================
//============================================================================
GdalFile::~GdalFile()
{
	Close();
}

//============================================================================
//============================================================================
bool GdalFile::Load(const char *file)
{
	const char *func = "GdalFile::Load() - ";

	Close();

	m_pds = (GDALDataset *) GDALOpen(file, GA_ReadOnly);
    if (m_pds == NULL)
    {
		LogError("%s Error: failed to open the file: %s", func, file);
		return false;
    }

	m_name = file;

	return InitDataset();
	/*
	m_w = m_pds->GetRasterXSize();
	m_h = m_pds->GetRasterYSize();
	m_c = m_pds->GetRasterCount();

	if (m_pds->GetGeoTransform( m_gt ) != CE_None)
	{
		Msg(pErrMsg, "Error: failed to get the geo transform for file: %s", file);
		return false;
    }

	LogTrace("w: %d, h: %d, rasters: %d", m_w, m_h, m_c);
	LogTrace("Origin = (%.6f,%.6f)", m_gt[0], m_gt[3]);
	LogTrace("Pixel Size = (%.6f,%.6f)", m_gt[1], m_gt[5]);

	m_extents.l = m_gt[0]; // lon
	m_extents.t = m_gt[3]; // lat

	//m_extents.r = m_extents.l + m_gt[1]*m_w;
	//m_extents.b = m_extents.t + m_gt[5]*m_h;
	m_extents.r = m_gt[0] + m_w*m_gt[1] + m_h*m_gt[2];
	m_extents.b = m_gt[3] + m_w*m_gt[4] + m_h*m_gt[5];

	LogTrace("l: %.2f, t: %.2f, r: %.2f, b: %.2f", m_extents.l, m_extents.t, m_extents.r, m_extents.b);
	*/

	return true;
}

#include "ogrsf_frmts.h"

//============================================================================
//============================================================================
bool GdalFile::LoadBingWms(const Extents &ext)
{
    //const char *func = "GdalFile::LoadBingWms() - ";

    //Testing real WMS server. Tile level not used.
    //Need to write out the file with the bounds we want
    //if ( FILE *ffile = fopen( TEMPFILE_WMSDATA, "w" ) )
    char boundString[64];
    std::string str = "<GDAL_WMS>";
    str += "<Service name=\"WMS\">";
    str += "<Version>1.1.1</Version>";
    str += "<ServerUrl>http://wms.onterrasystems.com/WMSService.svc/ec65eceb82db4599a0045a5748da8427/WMSLatLon?</ServerUrl>";
    str += "<ImageFormat>image/png</ImageFormat>";
    str += "<Layers>OnTerraWMS</Layers>";
    str += "<Styles>AerialWithLabels</Styles>";
    //str += "<Styles>Aerial</Styles>";
    str += "<SRS>EPSG:3857</SRS>";
    str += "</Service>";
    str += "<DataWindow>";
    str += "<UpperLeftX>";
    //sprintf( boundString, "%le", ext.l );
    sprintf( boundString, "%f", ext.l );
    str += boundString;	// lo X
    str += "</UpperLeftX>";
    str += "<UpperLeftY>";
    //sprintf( boundString, "%le", ext.t );
    sprintf( boundString, "%f", ext.t );
    str += boundString;	// hi Y
    str += "</UpperLeftY>";
    str += "<LowerRightX>";
    //sprintf( boundString, "%le", ext.r );
    sprintf( boundString, "%f", ext.r );
    str += boundString;	// hi X
    str += "</LowerRightX>";
    str += "<LowerRightY>";
    //sprintf( boundString, "%le", ext.b );
    sprintf( boundString, "%f", ext.b );
    str += boundString;	// lo Y
    str += "</LowerRightY>";
    str += "<SizeX>";
    //sprintf( boundString, "%d", OUTPUT_PIXEL_SIZE );
    sprintf( boundString, "%d", OUTPUT_PIXEL_SIZE * 2 );
    str += boundString;
    str += "</SizeX>";
    str += "<SizeY>";
    str += boundString;
    str += "</SizeY>";
    str += "</DataWindow>";
    str += "<Projection>EPSG:";
    sprintf( boundString, "%d", EPSG_DESIRED_SPATIALREF );
    str += boundString;	// EPSG value
    str += "</Projection>";
    str += "<ClampRequests>false</ClampRequests>";
    str += "<BandsCount>3</BandsCount>";
	//str += "<Timeout>300</Timeout>";
	//str += "<UserAgent>GDAL WMS driver (http://www.gdal.org/frmt_wms.html)</UserAgent>";
	//str += "<ZeroBlockOnServerException>true</ZeroBlockOnServerException>";

    str += "</GDAL_WMS>";

    LogTrace("wms: %s", str.c_str());
    //OGRDataSource *ds = OGRSFDriverRegistrar::Open(str.c_str());

    m_pds = (GDALDataset *) GDALOpen( str.c_str(), GA_ReadOnly );
	if (m_pds == NULL)
    {
		LogError("%s failed to load wms with extents: %s", ext.asStr().c_str());
        return false;
    }

	m_name = "wms - ";
	m_name += ext.asStr();

	if (!InitDataset()) return false;

	saveToTiff("wms.tiff");

	return true;
    /*
    GDALDataset *wmsDataset = (GDALDataset *) GDALOpen( str.c_str(), GA_ReadOnly );
    if ( wmsDataset == NULL )
    {
		LogError("%s failed to load wms with extents: %s", ext.asStr().c_str());
        return false;
    }

    // Make a copy of the raster and save it as GeoTiff
    dataSource = TEMPFILE_BASEIMAGE;
    GDALDriverManager *Mgr = GetGDALDriverManager();
    GDALDriver *driver = Mgr->GetDriverByName( "GTiff" );
    if ( driver == NULL )
    {
        std::cout << "Unable to obtain GDAL driver for GeoTiff." << std::endl;
        GDALClose ( wmsDataset );
        return false;
    }
    char *options[] = {"TILED=NO", NULL};
    GDALDataset *newDataset = driver->CreateCopy( dataSource.c_str(), wmsDataset, false, options, NULL, NULL );
    if ( newDataset == NULL )
    {
        std::cout << dataSource << CPLGetLastErrorMsg( ) << std::endl;
        std::cout << "Unable to copy " << wmsDataset->GetRasterCount() << " raster bands into new GeoTiff image." << std::endl;
        GDALClose ( wmsDataset );
        return false;
    }
    std::cout << "Created intermediate image file with size: x= "
        << newDataset->GetRasterXSize() << " and y= " << newDataset->GetRasterYSize();
    std::cout << " and with " << newDataset->GetRasterCount() << " bands." << std::endl;
    GDALClose( newDataset );
    GDALClose( wmsDataset );


    try
    {
        // Base raster
        feature_type_style raster_style;

        rule raster_style_rule;
        raster_symbolizer sym;
        sym.set_opacity( 1.0 );
        sym.set_scaling_method( SCALING_BILINEAR );
        raster_style_rule.append( sym );
        raster_style.add_rule(raster_style_rule);

        map->insert_style("raster", raster_style);

        // base raster
        {
            parameters p;
            p["type"] = "raster";
            p["file"] = TEMPFILE_BASEIMAGE;
            p["lox"] = m_mapExtents[0];
            p["hix"] = m_mapExtents[1];
            p["loy"] = m_mapExtents[2];
            p["hiy"] = m_mapExtents[3];

            layer lyr( "raster" );
            lyr.set_datasource( datasource_cache::instance().create( p ) );
            lyr.add_style( "raster" );
            lyr.set_srs( m_finalProj4String.c_str() );
            map->addLayer(lyr);
        }
    }
    catch( const std::exception & ex )
    {
        std::cerr << "### ProcessImageWMS: " << ex.what() << std::endl;
        std::cout << "It is possible the the Mapnik base directory is not specified properly." << std::endl
        << m_mapnikPath << std::endl;
        return false;
    }
    return true;
	*/
}

//============================================================================
//============================================================================
bool GdalFile::LoadBingTms(const Extents &ext, int tileLevel)
{
	Close();

	char boundString[64];
    std::string str = "<GDAL_WMS>";
    //str += "<Service name=\"TMS\">";
    //Mapquest
    //str += "<ServerUrl>http://otile1.mqcdn.com/tiles/1.0.0/sat/${z}/${x}/${y}.png</ServerUrl>";
    //Bing maps
    str += "<Service name=\"VirtualEarth\">";
    str += "<ServerUrl>http://ecn.t0.tiles.virtualearth.net/tiles/a${quadkey}?g=1239</ServerUrl>";
    //str += "<ServerUrl>http://a${server_num}.tiles.virtualearth.net/tiles/a${quadkey}.jpeg?g=90</ServerUrl>";
    str += "</Service>";
    str += "<DataWindow>";
    str += "<UpperLeftX>";
    sprintf( boundString, "%le", -20037508.34 );
    str += boundString;    // lo X
    str += "</UpperLeftX>";
    str += "<UpperLeftY>";
    sprintf( boundString, "%le", 20037508.34 );
    str += boundString;    // hi Y
    str += "</UpperLeftY>";
    str += "<LowerRightX>";
    sprintf( boundString, "%le", 20037508.34 );
    str += boundString;    // hi X
    str += "</LowerRightX>";
    str += "<LowerRightY>";
    sprintf( boundString, "%le", -20037508.34 );
    str += boundString;    // lo Y
    str += "</LowerRightY>";
    str += "<TileLevel>";
    sprintf( boundString, "%d", tileLevel );
    str += boundString;
    str += "</TileLevel>";
    str += "<TileCountX>1</TileCountX>";
    str += "<TileCountY>1</TileCountY>";
    str += "<YOrigin>top</YOrigin>";
    str += "</DataWindow>";
    str += "<Projection>EPSG:";
    sprintf( boundString, "%d", EPSG_DESIRED_SPATIALREF );
    str += boundString;    // EPSG value
    str += "</Projection>";
    str += "<BlockSizeX>256</BlockSizeX>";
    str += "<BlockSizeY>256</BlockSizeY>";
    str += "<BandsCount>3</BandsCount>";
    str += "<Cache />";
    str += "</GDAL_WMS>";

    m_pds = (GDALDataset *) GDALOpen( str.c_str(), GA_ReadOnly );
	if (m_pds == NULL)
    {
		LogError("%s failed to load wms with extents: %s", ext.asStr().c_str());
        return false;
    }

	m_name = "tms - ";
	m_name += ext.asStr();

	if (!InitDataset()) return false;

	saveToTiff("tms.tiff");


	return false;

	/*
	int Field_Render::FindBestTilelevel() const
{

    double extentRange[2];
    extentRange[0] = m_mapExtents[1] - m_mapExtents[0];
    extentRange[1] = m_mapExtents[3] - m_mapExtents[2];
    double maxRange = extentRange[0] > extentRange[1] ? extentRange[0] : extentRange[1];
    // See https://trac.openstreetmap.org/browser/subversion/applications/rendering/mapnik/zoom-to-scale.txt
    double maxScale = 2132.72958385;
    int tileLevel = 18;

    while ( tileLevel > 1 )
    {
        if ( maxRange <= maxScale )
        {
            break;
        }
        --tileLevel;
        maxScale *= 2.0;
    }
    std::cout << "Maximum range: " << maxRange << ". Tile Level: " << tileLevel << std::endl;
    return tileLevel;
}
	*/

}

//============================================================================
//============================================================================
bool GdalFile::WmsTest()
{
    //std::string str = "<GDAL_WMS><Service name=\"WMS\"><Version>1.1.1</Version><ServerUrl>http://onearth.jpl.nasa.gov/wms.cgi?</ServerUrl><SRS>EPSG:4326</SRS><ImageFormat>image/jpeg</ImageFormat><Layers>modis,global_mosaic</Layers><Styles></Styles></Service><DataWindow><UpperLeftX>-180.0</UpperLeftX><UpperLeftY>90.0</UpperLeftY><LowerRightX>180.0</LowerRightX><LowerRightY>-90.0</LowerRightY><SizeX>2666666</SizeX><SizeY>1333333</SizeY></DataWindow><Projection>EPSG:4326</Projection><BandsCount>3</BandsCount></GDAL_WMS>";

    std::string str = "<GDAL_WMS>";
        str += "<Service name=\"WMS\">";
        str += "<Version>1.1.1</Version>";
            str += "<ServerUrl>http://sedac.ciesin.columbia.edu/mapserver/map/GPWv3?</ServerUrl>";
    str += "<SRS>EPSG:4326</SRS>";
            str += "<ImageFormat>image/jpeg</ImageFormat>";
            str += "<Layers>GPWv3_1990_Pop_Density_tif</Layers>";
            str += "<Styles></Styles>";
        str += "</Service>";
        str += "<DataWindow>";
            str += "<UpperLeftX>-180.0</UpperLeftX>";
            str += "<UpperLeftY>85.0</UpperLeftY>";
            str += "<LowerRightX>180.0</LowerRightX>";
            str += "<LowerRightY>-60.0</LowerRightY>";
            str += "<SizeX>36000</SizeX>";
            str += "<SizeY>14500</SizeY>";
        str += "</DataWindow>";
        str += "<Projection>EPSG:4326</Projection>";
        str += "<BandsCount>3</BandsCount>";
    str += "</GDAL_WMS>";

    m_pds = (GDALDataset *) GDALOpen( str.c_str(), GA_ReadOnly );
    if (m_pds == NULL)
    {
        LogError("%s failed to load wms test");
        return false;
    }

    m_name = "wms - test";

    if (!InitDataset()) return false;

    return saveToTiff("wms.tiff");

}

//============================================================================
//============================================================================
bool GdalFile::InitDataset()
{
	const char *func = "GdalFile::InitDataset() - ";

	m_w = m_pds->GetRasterXSize();
	m_h = m_pds->GetRasterYSize();
	m_c = m_pds->GetRasterCount();

	if (m_pds->GetGeoTransform( m_gt ) != CE_None)
	{
		LogError("%s Error: failed to get the geo transform from dataset %s", func, m_name.c_str());
		return false;
    }


	m_extents.l = m_gt[0]; // lon
	m_extents.t = m_gt[3]; // lat
	m_extents.r = m_gt[0] + m_w*m_gt[1] + m_h*m_gt[2];
	m_extents.b = m_gt[3] + m_w*m_gt[4] + m_h*m_gt[5];



	// TODO: need to time this, if the stats are not calculated and no time hit, then it would be fine to init stats here, otherwise
	// just get stats when getting hmap
	/*
	GDALRasterBand *pBand = m_pds->GetRasterBand(1);
	if (pBand == NULL)
	{
		return true; // no stats
	}

	pBand->GetStatistics(1, 1, &m_stats.min, &m_stats.max, &m_stats.mean, &m_stats.stddev);
	*/



	LogTrace("%s info...", m_name.c_str());
	LogTrace("w: %d, h: %d, rasters: %d", m_w, m_h, m_c);
	LogTrace("Origin = (%.6f,%.6f)", m_gt[0], m_gt[3]);
	LogTrace("Pixel Size = (%.6f,%.6f)", m_gt[1], m_gt[5]);
	LogTrace("%s extets: %s", func, m_extents.asStr().c_str() );

    /*
    char **meta = m_pds->GetDriver()->GetMetadata();

    int i=0;
    while (meta[i] != NULL)
    {
       LogTrace( "MetaData: %s", meta[i]);
       i++;
    }
    */

    LogTrace("Gdal Driver: %s/%s", m_pds->GetDriver()->GetDescription(), m_pds->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
	if (m_pds->GetProjectionRef() != NULL)
        LogTrace( "Projection is %s", m_pds->GetProjectionRef());

	return true;
}

//============================================================================
//============================================================================
bool GdalFile::GetChannelDataType(int channelid, GDALDataType *pType) const
{
	if (!ValidChannel(channelid)) return false;

	GDALRasterBand *pBand = m_pds->GetRasterBand(channelid);
	if (!pBand)
	{
		return false;
	}

	*pType = pBand->GetRasterDataType();
	return true;
}

//============================================================================
//============================================================================
bool GdalFile::GetChannelInfo(int channelid, std::string *pInfo) const
{
	GDALRasterBand  *pBand;
	int			    nXSize=0, nYSize=0;
    int             nBlockXSize, nBlockYSize;
    int             bGotMin, bGotMax;
    double          adfMinMax[2];

	if (!ValidChannel(channelid)) return false;

    pBand = m_pds->GetRasterBand( channelid );
	if (pBand == NULL)
	{
		Msg(pInfo, "Failed to get the Channel Object");
		return false;
	}

	nXSize = pBand->GetXSize();
	nYSize = pBand->GetYSize();
	Msg(pInfo, "Size=%dx%d\n", nXSize, nYSize);

    pBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
    Msg(pInfo, "Block=%dx%d Type=%s, ColorInterp=%s\n",
                nBlockXSize, nBlockYSize,
                GDALGetDataTypeName(pBand->GetRasterDataType()),
                GDALGetColorInterpretationName(pBand->GetColorInterpretation()) );

	adfMinMax[0] = pBand->GetMinimum( &bGotMin );
    adfMinMax[1] = pBand->GetMaximum( &bGotMax );
    if( ! (bGotMin && bGotMax) ) GDALComputeRasterMinMax((GDALRasterBandH)pBand, TRUE, adfMinMax);

    Msg(pInfo, "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );

    if (pBand->GetOverviewCount() > 0) Msg(pInfo, "Band has %d overviews.\n", pBand->GetOverviewCount() );

    if (pBand->GetColorTable() != NULL)
	{
		Msg(pInfo, "Band has a color table with %d entries.\n", pBand->GetColorTable()->GetColorEntryCount() );
	}

	return true;
}

//============================================================================
//============================================================================
bool GdalFile::ValidChannel(int channelid) const
{
	const char *func = "GdalFile::ValidChannel()";
	if (m_pds == NULL)
	{
		logError(func, "no dataset");
		return false;
	}

	if (m_c < 1)
	{
		logError(func, "no channel data");
		return false;
	}

	if (channelid > m_c || channelid < 1)
	{
		LogError("%s channel %d is out of bounds, raster count: %d. file: %s", func, channelid, m_c, m_name.c_str());
		return false;
	}

	return true;
}

//============================================================================
//============================================================================
bool GdalFile::saveToTiff(const char *path)
{
	const char *func = "GdalFile::saveToTiff() - ";

	LogTrace("");
	LogTrace("%s saving to tiff: %s...", func, path);

	// Make a copy of the raster and save it as GeoTiff
    GDALDriverManager *Mgr = GetGDALDriverManager();
    GDALDriver *driver = Mgr->GetDriverByName( "GTiff" );
    if ( driver == NULL )
    {
		LogError("%s Unable to obtain GDAL driver for GeoTiff.", func);
		return false;
	}

    char* options[2] = {"TILED=NO", NULL};
    GDALDataset *newDataset = driver->CreateCopy(path, m_pds, false, options, NULL, NULL);
    if ( newDataset == NULL )
    {
		LogError("%s Failed to copy to %s, last error: %s", func, path, CPLGetLastErrorMsg());
        return false;
	}

	GDALClose(newDataset);

	return true;
	/*
	std::cout << "Created intermediate image file with size: x= "
            << newDataset->GetRasterXSize() << " and y= " << newDataset->GetRasterYSize();
        std::cout << " and with " << newDataset->GetRasterCount() << " bands." << std::endl;
        GDALClose( newDataset );
        GDALClose( wmsDataset );
	*/
}

//============================================================================
//============================================================================
bool GdalFile::GetRGB(MemBuf *pbuf, bool addAlpha) const
{
	const char *func = "GdalFile::GetRGB() - ";
	if (!ValidImage(3)) return false;

	int bytesPerPix = 3;
	if (addAlpha) bytesPerPix = 4;

	pbuf->SetSize(bytesPerPix, m_w, m_h);

	LogTrace("", func);
	LogTrace("", func);
	LogTrace("%s loading RGB data from raster...", func);

	for (int i=1; i<=3; i++)
	{
		GDALRasterBand *pBand = m_pds->GetRasterBand(i);
		if (pBand == NULL)
		{
			//Msg(pInfo, "Failed to get the Channel Object");
			return false;
		}

		int w = pBand->GetXSize();
		int h = pBand->GetYSize();
		if (m_w != w || m_h != h)
		{
			return false;
		}

		BYTE *buf = (BYTE *)pbuf->GetBuf();
		buf = &buf[i-1];
		if (pBand->RasterIO(GF_Read, 0, 0, w, h, buf, w, h, GDT_Byte, bytesPerPix, 0) == CE_Failure)
		{
			LogError("%s RasterIO failed for channel: %d. file: %s", func, i, m_name.c_str());
			return false;
		}
	}

	return true;

	//! Eight bit unsigned integer            GDT_Byte = 1,
    //! Sixteen bit unsigned integer          GDT_UInt16 = 2,
    //! Sixteen bit signed integer            GDT_Int16 = 3,
    //! Thirty two bit unsigned integer       GDT_UInt32 = 4,
    //! Thirty two bit signed integer         GDT_Int32 = 5,
    //! Thirty two bit floating point         GDT_Float32 = 6,
    //! Sixty four bit floating point         GDT_Float64 = 7,
    //! Complex Int16                         GDT_CInt16 = 8,
    //! Complex Int32                         GDT_CInt32 = 9,
    //! Complex Float32                       GDT_CFloat32 = 10,
    //! Complex Float64                       GDT_CFloat64 = 11,
}

//============================================================================
//============================================================================
bool GdalFile::GetHmap(MemBuf *pbuf, Stats *pstats) const
{
	const char *func = "GdalFile::GetHmap() - ";

	if (!ValidImage(1)) return false;

	pbuf->SetSize(sizeof(double), m_w, m_h);


	GDALRasterBand *pBand = m_pds->GetRasterBand(1);
	if (pBand == NULL)
	{
		logError(func, "Failed to get the channel object.");
		return false;
	}

	int w = pBand->GetXSize();
	int h = pBand->GetYSize();
	if (m_w != w || m_h != h)
	{
		LogError("%s band width or height missmatch, band(%d, %d), file(%d, %d), file: %s", func, w, h, m_w, m_h, m_name.c_str());
		return false;
	}

	double *buf = (double *)pbuf->GetBuf();
	if (pBand->RasterIO(GF_Read, 0, 0, w, h, buf, w, h, GDT_Float64, 0, 0) == CE_Failure)
	{
		logError(func, "RasterIO failed.");
		return false;
	}

	if (pstats)
	{
		pBand->GetStatistics(1, 1, &pstats->_min, &pstats->_max, &pstats->_mean, &pstats->_sdev);
		//pBand->GetStatistics(1, 1, &m_stats.min, &m_stats.max, &m_stats.mean, &m_stats.stddev);
	}

	return true;
}

//============================================================================
//============================================================================
bool GdalFile::ValidImage(int numChannels) const
{
	if (m_pds == NULL)
	{
		return false;
	}

	if (m_w <= 0 || m_h <= 0)
	{
		return false;
	}

	if (numChannels > 0)
	{
		if (GetChannelCount() != numChannels)
		{
			return false;
		}
	}


	return true;
}

/*
void GdalFile::GetExtentsUtm(UtmPos *pTL, UtmPos *pTR, UtmPos *pBL, UtmPos *pBR)
{
	//m_extents.l = m_gt[0]; // lat
	//m_extents.t = m_gt[3]; // lon

	if (pTL != NULL)
	{
		pTL->SetPos(m_extents.t, m_extents.l);
	}
	if (pTR != NULL)
	{
		pTR->SetPos(m_extents.t, m_extents.r);
	}
	if (pBL != NULL)
	{
		pBL->SetPos(m_extents.b, m_extents.l);
	}
	if (pBR != NULL)
	{
		pBR->SetPos(m_extents.b, m_extents.r);
	}
}

void GdalFile::GetExtentsUtm(UtmPos *pTL, UtmPos *pBR)
{
	if (pTL != NULL)
	{
		pTL->SetPos(m_extents.t, m_extents.l);
	}
	if (pBR != NULL)
	{
		pBR->SetPos(m_extents.b, m_extents.r);
	}
}
*/

//============================================================================
//============================================================================
void GdalFile::Msg(std::string *pMsg, const char *format, ...)
{
	/*if (pMsg == NULL) return;

	char buffer[10240];
	va_list args;

	va_start(args, format);
	vsprintf_s(buffer, 10240, format, args);
	va_end(args);

	*pMsg += buffer;*/
}

//============================================================================
//============================================================================
void GdalFile::Close()
{
	if (m_pds)
	{
		GDALClose(m_pds);
		m_pds = NULL;
	}

	m_name = "";
}

//============================================================================
//============================================================================
void GdalFile::logError(const char *func, const char *msg) const
{
	LogError("%s %s file: %s", func, msg, m_name.c_str());
}
