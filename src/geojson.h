#ifndef GEOJSON_H
#define GEOJSON_H

#include "geopoly.h"

class OGRDataSource;
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

    int getGeometry(PGlObj parent);
    int getGeometry(std::vector<PGlObj> *vpolys);

protected:
    void close();

    bool convertGeometry(OGRGeometry *geom, OGRFeatureDefn *poFDefn, OGRFeature *poFeature, std::vector<PGlObj> *vpolys);
    PGlObj getPoly(OGRPolygon *ogrpoly);
    void initFields(GeoPoly *poly, OGRFeatureDefn *poFDefn, OGRFeature *poFeature);

protected:
    OGRDataSource *_ds;
};

#endif
