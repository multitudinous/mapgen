#ifndef GEOJSON_H
#define GEOJSON_H

#include "geopoly.h"

class OGRDataSource;
class OGRLayer;
class OGRPolygon;
class OGRFeatureDefn;
class OGRFeature;
class OGRGeometry;

class GeoJson
{
public:
    GeoJson();
    virtual ~GeoJson();

    bool loadFile(const char *jsonfile);

    int getGeometry(int lyrnum, PGlObj parent);
    int getGeometry(const char *lyrname, PGlObj parent);
    int getGeometry(int lyrnum, std::vector<PGlObj> *vpolys);
    int getGeometry(const char *lyrname, std::vector<PGlObj> *vpolys);

protected:
    void close();

    int getGeometry(OGRLayer *plyr, std::vector<PGlObj> *vpolys);
    bool convertGeometry(OGRGeometry *geom, OGRFeatureDefn *poFDefn, OGRFeature *poFeature, std::vector<PGlObj> *vpolys);
    PGlObj getPoly(OGRPolygon *ogrpoly);
    void initFields(GeoPoly *poly, OGRFeatureDefn *poFDefn, OGRFeature *poFeature);

    void logLayerNames();

protected:
    OGRDataSource *_ds;
};

#endif
