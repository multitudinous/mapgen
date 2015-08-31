#ifndef GEOJSON_H
#define GEOJSON_H

#include "geopoly.h"

class OGRDataSource;
class OGRLayer;
class OGRPolygon;
class OGRFeatureDefn;
class OGRFeature;
class OGRGeometry;
class GDALDataset;

class GeoJson
{
public:
    struct EpsgTrans
    {
        int epsgFr;
        int epsgTo;

        EpsgTrans()
        {
            epsgFr = 0; // no transform
            epsgTo = 0;
        }

        EpsgTrans(int epsgfr)
        {
            epsgFr = epsgfr; // no transform
            epsgTo = 3857; // default;
        }

        EpsgTrans(int epsgfr, int epsgto)
        {
            epsgFr = epsgfr; // no transform
            epsgTo = epsgto; // default;
        }
    };
public:
    GeoJson();
    virtual ~GeoJson();

    bool loadFile(const char *jsonfile);
    void setTransform(const EpsgTrans &trans) { _trans = trans;  }

    int getGeometry(int lyrnum, PGlObj parent);
    int getGeometry(const char *lyrname, PGlObj parent);
    int getGeometry(int lyrnum, std::vector<PGlObj> *vpolys);
    int getGeometry(const char *lyrname, std::vector<PGlObj> *vpolys);

    // default values convert unprojected WGS84 (4326) to 3857 used by bing
    static Point2d convertPt(const Point2d &ptin, const EpsgTrans &trans);
    static Point2d convertPt(const Point2d &ptin, int inEPSG=4326, int outEPSG=3857); 

protected:
    void close();

    int getGeometry(OGRLayer *plyr, std::vector<PGlObj> *vpolys);
    bool convertGeometry(OGRGeometry *geom, OGRFeatureDefn *poFDefn, OGRFeature *poFeature, std::vector<PGlObj> *vpolys);
    PGlObj getPoly(OGRPolygon *ogrpoly);
    void initFields(GeoPoly *poly, OGRFeatureDefn *poFDefn, OGRFeature *poFeature);

    void logLayerNames();

protected:
    GDALDataset *_ds;
    EpsgTrans _trans;
};

#endif
