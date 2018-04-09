#ifndef GEOPOLY_H
#define GEOPOLY_H

#include "geoobj.h"
#include "point2d.h"
#include "vbo.h"

class GeoPoly : public GeoObj
{
public:
    GeoPoly();
    GeoPoly(const GeoPoly &poly);

    virtual void draw(DrawData *pdd);
    virtual void computeBox(DrawData *pdd, box3d *pbox);
    virtual void computeBoxNoLabel(box3d *pbox) const;
    virtual void initLabels(DrawData *pdd);

    void push_back(const Point2d &pt);
	void pop_back(); // remove the last element
    size_t ptCount() const { return _pts.size(); }
    size_t size() const { return _pts.size(); }
    Point2d at(int i) const;
    const std::vector<Point2d>& getPts() const { return _pts; }

    void insertField(const char *name, int v);
    void insertField(const char *name, double v);
    void insertField(const char *name, const char *v);

    virtual bool haveFeature(DrawData *pdd, std::string *value);

    bool getFeatureValue(const char *feature, std::string *value);
    virtual bool getFeatureValue(const std::string &feature, std::string *value);
    virtual bool getFeatureValue(const std::string &feature, double *value);

    int findIntersections(const Point2d &p1, const Point2d &p2, ListPt2d *plist, bool ptOnSeg=true, bool ptOnPoly=true);

    Point2d findCentroid();

    virtual void scale(double x, double y, double z=1.0);
    virtual void translate(double x, double y, double z=0.0);
    virtual void moveFromTo(const vec3d &fr, const vec3d &to);

    virtual GlObj* clone() const;

	std::vector< shared_ptr<GeoPoly> >& getInnerRings() { return _innerRings; }

protected:
    void drawFill(DrawData *pdd);
    void drawOutline(DrawData *pdd);
    void drawMask(DrawData *pdd);
    void drawLabel(DrawData *pdd);
    void drawDebug(DrawData *pdd);

    bool getDrawFeature(DrawData *pdd, std::string *str);
    bool getDrawFeature(DrawData *pdd, char *str);
    Point2d findLabelPt(DrawData *pdd);
    bool findLabelPointFromIsects(DrawData *pdd, const Point2d &left, const Point2d &right, Point2d *result);


	void drawOutlineBasic(DrawData *pdd, const Rgbf &color, float linew);
	void drawOutlineShader(DrawData *pdd, const Rgbf &color, float linew);
	void drawOutlineVaser(DrawData *pdd, const Rgbf &color, float linew);

protected:
    ListPt2d _pts;

    typedef std::map<std::string, int> MapFieldsInt;
    typedef std::map<std::string, double> MapFieldsDbl;
    typedef std::map<std::string, std::string> MapFieldsStr;
    MapFieldsInt _fieldsInt;
    MapFieldsDbl _fieldsDbl;
    MapFieldsStr _fieldsStr;

    Point2d _labelPt;

    PVbo _vboOutline;

	std::vector< shared_ptr<GeoPoly> > _innerRings;
};

typedef shared_ptr<GeoPoly> PGeoPoly;

#endif
