#include "geojson.h"
#include "ogrsf_frmts.h"

//============================================================================
//============================================================================
GeoJson::GeoJson()
{
    _ds = NULL;
}

//============================================================================
//============================================================================
GeoJson::~GeoJson()
{
    close();
}

//============================================================================
//============================================================================
bool GeoJson::loadFile(const char *jsonfile)
{
    const char *func = "GeoJson::loadFile() - ";
    close();

    //const char *pszDriverName = "GeoJSON";
    //OGRSFDriver  *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName );
    //_ds = poDriver->Open(jsonfile, 0);

    _ds = OGRSFDriverRegistrar::Open(jsonfile);
    if (_ds == NULL )
    {
        LogError("%s failed to open jsonfile: %s", func, jsonfile);
        return false;
    }

    return true;
}

//============================================================================
//============================================================================
int GeoJson::getGeometry(int lyrnum, PGlObj parent)
{
    std::vector<PGlObj> polys;
    int ret = getGeometry(lyrnum, &polys);

    for (unsigned int i = 0; i<polys.size(); i++)
    {
        parent->insertBack(polys[i]);
    }

    return ret;
}

//============================================================================
//============================================================================
int GeoJson::getGeometry(const char *lyrname, PGlObj parent)
{
    std::vector<PGlObj> polys;
    int ret = getGeometry(lyrname, &polys);

    for (unsigned int i = 0; i<polys.size(); i++)
    {
        parent->insertBack(polys[i]);
    }

    return ret;
}

//============================================================================
//============================================================================
int GeoJson::getGeometry(int lyrnum, std::vector<PGlObj> *vpolys)
{
    if (!_ds) return 0;
    if (_ds->GetLayerCount() <= 0) return 0;

    int lyrcount = _ds->GetLayerCount();
    if (lyrnum >= lyrcount)
    {
        LogError("layer number %d is not valid given the total layer count %d", lyrnum, lyrcount);
        return 0;
    }

    OGRLayer *lyr = _ds->GetLayer(lyrnum);
    return getGeometry(lyr, vpolys);
}

//============================================================================
//============================================================================
int GeoJson::getGeometry(const char *lyrname, std::vector<PGlObj> *vpolys)
{
    if (!_ds) return 0;
    if (_ds->GetLayerCount() <= 0) return 0;

    OGRLayer *lyr =  _ds->GetLayerByName(lyrname);
    if (!lyr)
    {
        LogError("layer named $s not found", lyrname);
        logLayerNames();
        return 0;
    }

    return getGeometry(lyr, vpolys);
}

//============================================================================
//============================================================================
/*
int GeoJson::getGeometry(std::vector<PGlObj> *vpolys)
{
    if (!_ds) return 0;
    if (_ds->GetLayerCount() <= 0) return 0;

    int lyrCount = _ds->GetLayerCount();
    for (int i = 0; i < lyrCount; i++)
    {
        OGRLayer *plyr = _ds->GetLayer(i);
        const char *name = plyr->GetName();
        std::string lyrname = name;
    }

    OGRLayer *plyr = _ds->GetLayer(0);
    OGRFeature *poFeature;
    int countstart = (int)vpolys->size();

    LogTrace("parsing layer 0..");
    plyr->ResetReading();
    while ((poFeature = plyr->GetNextFeature()) != NULL)
    {

        OGRFeatureDefn *poFDefn = plyr->GetLayerDefn();

        // Starting with OGR 1.11, several geometry fields can be associated to a feature.

        int iGeomField;
        int nGeomFieldCount;

        LogTrace("parsing geometry...");
        nGeomFieldCount = poFeature->GetGeomFieldCount();
        for (iGeomField = 0; iGeomField < nGeomFieldCount; iGeomField++)
        {
            OGRGeometry *geom = poFeature->GetGeomFieldRef(iGeomField);
            convertGeometry(geom, poFDefn, poFeature, vpolys);
        }

        OGRFeature::DestroyFeature(poFeature);
    }

    // polys loaded
    return (int)vpolys->size() - countstart;
}
*/

//============================================================================
//============================================================================
int GeoJson::getGeometry(OGRLayer *plyr, std::vector<PGlObj> *vpolys)
{
    if (!plyr) return 0;

    OGRFeature *poFeature;
    int countstart = (int)vpolys->size();

    LogTrace("parsing layer ..");
    plyr->ResetReading();
    while( (poFeature = plyr->GetNextFeature()) != NULL )
    {

        OGRFeatureDefn *poFDefn = plyr->GetLayerDefn();

        /*
        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        if( poGeometry != NULL
            && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
        {
            OGRPoint *poPoint = (OGRPoint *) poGeometry;
            LogTrace( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
        }
        else
        {
            LogTrace( "no point geometry\n" );
        }
        */

        // Starting with OGR 1.11, several geometry fields can be associated to a feature.

        int iGeomField;
        int nGeomFieldCount;

        LogTrace("parsing geometry...");
       nGeomFieldCount = poFeature->GetGeomFieldCount();
       for(iGeomField = 0; iGeomField < nGeomFieldCount; iGeomField ++ )
       {
         OGRGeometry *geom = poFeature->GetGeomFieldRef(iGeomField);
         convertGeometry(geom, poFDefn, poFeature, vpolys);
       }

       OGRFeature::DestroyFeature( poFeature );
    }

    // polys loaded
    return (int)vpolys->size() - countstart;
}

//============================================================================
//============================================================================
bool GeoJson::convertGeometry(OGRGeometry *geom, OGRFeatureDefn *poFDefn, OGRFeature *poFeature, std::vector<PGlObj> *vpolys)
{
    if (geom == NULL) return false;

    LogTrace("Geometry found with name: %s and type: %s", geom->getGeometryName(), OGRGeometryTypeToName(geom->getGeometryType()));

    if (geom->getGeometryType() == wkbPolygon)
    {
        PGlObj poly = getPoly((OGRPolygon *)geom);
        if (poly)
        {
            vpolys->push_back(poly);
            initFields((GeoPoly *)poly.get(), poFDefn, poFeature);
            return true;
        }
    }
    else if (geom->getGeometryType() == wkbMultiPolygon)
    {
        OGRMultiPolygon *multipoly = dynamic_cast<OGRMultiPolygon*>(geom);
        if (!multipoly)
        {
              LogError("failed to typecast to OGRMultiPolygon");
              return false;
        }


        int converted = 0;
        int count = multipoly->getNumGeometries();
        LogTrace("Multipolygon found with %d geometries", count);
        for (int i=0; i<count; i++)
        {
            OGRGeometry *geom2 = multipoly->getGeometryRef(i);
            if (convertGeometry(geom2, poFDefn, poFeature, vpolys)) converted++;
        }

        LogTrace("Multipolygon converted %d geometries", converted);
        if (count && count == converted) return true;
    }

    /*
    if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
    {
        OGRPoint *poPoint = (OGRPoint *) poGeometry;

       LogTrace( "%.3f,%3.f", poPoint->getX(), poPoint->getY() );
    }
    else
    {
        LogTrace( "no point geometry" );
    }
    */

    return false;

}

//============================================================================
//============================================================================
void GeoJson::close()
{
    if (_ds) OGRDataSource::DestroyDataSource( _ds );
    _ds = NULL;
}

//============================================================================
//============================================================================
PGlObj GeoJson::getPoly(OGRPolygon *ogrpoly)
{
    const char *func = "PGeoPoly GeoJson::getPoly() - ";
    GeoPoly *poly = new GeoPoly();
    PGlObj obj(poly);

    // make 2d
    ogrpoly->flattenTo2D();

    OGRLinearRing *ring = ogrpoly->getExteriorRing();
    if (!ring)
    {
        LogError("no outer ring for poly", func);
        return PGlObj();
    }

    for (int i=0; i<ring->getNumPoints(); i++)
    {
        Point2d ptin(ring->getX(i), ring->getY(i)); //pt->getZ(i)
        Point2d pt = convertPt(ptin, _trans);
        poly->push_back(pt);
    }


    // TODO: inner rings


    LogTrace("Polygon with %d points, interior rings: %d", poly->size(), ogrpoly->getNumInteriorRings());

    return obj;
}

//============================================================================
//============================================================================
void GeoJson::initFields(GeoPoly *poly, OGRFeatureDefn *poFDefn, OGRFeature *poFeature)
{
    LogTrace("fields...");
    for (int iField = 0; iField < poFDefn->GetFieldCount(); iField++)
    {
        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

        LogTrace("field %d name ref: %s", iField, poFieldDefn->GetNameRef());

        if( poFieldDefn->GetType() == OFTInteger )
        {
            LogTrace( "int : %d,", poFeature->GetFieldAsInteger( iField ) );
            poly->insertField(poFieldDefn->GetNameRef(), poFeature->GetFieldAsInteger(iField));
        }
        else if( poFieldDefn->GetType() == OFTReal )
        {
            LogTrace( "double: %.3f,", poFeature->GetFieldAsDouble(iField) );
            poly->insertField(poFieldDefn->GetNameRef(), poFeature->GetFieldAsDouble(iField));
        }
        else if( poFieldDefn->GetType() == OFTString )
        {
            LogTrace( "str: %s,", poFeature->GetFieldAsString(iField) );
            poly->insertField(poFieldDefn->GetNameRef(), poFeature->GetFieldAsString(iField));
        }
        else
       {
            LogTrace( "unktype str: %s,", poFeature->GetFieldAsString(iField) );
            poly->insertField(poFieldDefn->GetNameRef(), poFeature->GetFieldAsString(iField));
        }
    }
}

//============================================================================
//============================================================================
void GeoJson::logLayerNames()
{
    std::string strlyrs = "";
    for (int i = 0; i < _ds->GetLayerCount(); i++)
    {
        OGRLayer *lyr = _ds->GetLayer(i);
        if (lyr)
        {
            if (strlyrs.size() > 0) strlyrs += ",";
            strlyrs += lyr->GetName();
        }
    }

    LogTrace("Layer count: %d, Layer Names: %s", _ds->GetLayerCount(), strlyrs.c_str());
}

//============================================================================
//============================================================================
Point2d GeoJson::convertPt(const Point2d &ptin, const EpsgTrans &trans)
{
    return convertPt(ptin, trans.epsgFr, trans.epsgTo);
}

//============================================================================
//============================================================================
Point2d GeoJson::convertPt(const Point2d &ptin, int inEPSG, int outEPSG)
{
    if (inEPSG == 0)
    {
        return ptin;
    }

    OGRPoint *geompt = (OGRPoint *)OGRGeometryFactory::createGeometry(wkbPoint);
    geompt->setX(ptin.dX);
    geompt->setY(ptin.dY);

    OGRSpatialReference spatRefIn;
    spatRefIn.importFromEPSG(inEPSG);

    OGRSpatialReference spatRefOut;
    spatRefOut.importFromEPSG(outEPSG);

    OGRCoordinateTransformation *trans = OGRCreateCoordinateTransformation(&spatRefIn, &spatRefOut);

    geompt->transform(trans);
    Point2d newPt(geompt->getX(), geompt->getY());

    OGRCoordinateTransformation::DestroyCT(trans);
    OGRGeometryFactory::destroyGeometry(geompt);

    return newPt;
}