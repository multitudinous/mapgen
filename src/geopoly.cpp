#include "geopoly.h"
#include "gldraw.h"
#include "utlline2d.h"
#include <cstring> 

//============================================================================
//============================================================================
GeoPoly::GeoPoly()
{

}

//============================================================================
//============================================================================
GeoPoly::GeoPoly(const GeoPoly &poly) :
    GeoObj(poly)
{
   _pts = poly._pts;

    _fieldsInt  =  poly._fieldsInt;
    _fieldsDbl  =  poly._fieldsDbl;
    _fieldsStr  =  poly._fieldsStr;
    _labelPt    = poly._labelPt;
}

//============================================================================
//============================================================================
GlObj* GeoPoly::clone() const
{
    return new GeoPoly(*this);
}

//============================================================================
//============================================================================
void GeoPoly::draw(DrawData *pdd)
{
    GlObj::draw(pdd);
    // TODO: if this is null, use objects attributes
    // need routines for each items to precisely compute what attribute

    drawMask(pdd);
    drawFill(pdd);
    drawOutline(pdd);
    drawLabel(pdd);
    //drawDebug(pdd);
}

//============================================================================
//============================================================================
void GeoPoly::drawFill(DrawData *pdd)
{
    if (!DrawAttr::drawPolyFill(pdd->_drawAttr, _drawAttr)) return;
    Rgbf color = DrawAttr::colorPolyFill(pdd->_drawAttr, _drawAttr);

    glColor4fv(color.m_af);
    Gldraw::drawPolyConcaveVN(pdd->m_hGluTess, this);

}

//============================================================================
//============================================================================
void GeoPoly::drawOutline(DrawData *pdd)
{
    if (!DrawAttr::drawPolyOutline(pdd->_drawAttr, _drawAttr)) return;
    Rgbf color = DrawAttr::colorPolyOutline(pdd->_drawAttr, _drawAttr);
    float linew = DrawAttr::lineWidth(pdd->_drawAttr, _drawAttr);

    //glColor4fv(color.m_af);
    //glLineWidth(linew);
    Gldraw::drawLineLoop(getPts(), linew, color.GetR(), color.GetG(), color.GetB());
}

//============================================================================
//============================================================================
void GeoPoly::drawMask(DrawData *pdd)
{
    if (!DrawAttr::drawPolyMasked(pdd->_drawAttr, _drawAttr)) return;
    const GlObjList* maskList = DrawAttr::maskPoly(pdd->_drawAttr, _drawAttr);

    if (!maskList) return;
    if (maskList->size() <= 0) return;

    Gldraw::drawMask(pdd, maskList, getPts());

    /*
    for (unsigned int i=0; i<maskList->size(); i++)
    {
        glClearStencil(0);
        glClear(GL_STENCIL_BUFFER_BIT);

        glEnable(GL_STENCIL_TEST);
        glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

        glStencilFunc(GL_ALWAYS, 1, -1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        Gldraw::drawPolyConcaveVN(pdd->m_hGluTess, this);
        glStencilFunc(GL_EQUAL, 1, -1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
        (*maskList)[i]->draw(pdd);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_STENCIL_TEST);
    }
    */
}

//============================================================================
//============================================================================
void GeoPoly::drawLabel(DrawData *pdd)
{
    if (!DrawAttr::drawLabels(pdd->_drawAttr, _drawAttr)) return;

   char str[64];
   if (!getDrawFeature(pdd, str)) return;

   GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
   if (!pfont) return;

   Rgbf color = DrawAttr::colorLabels(pdd->_drawAttr, _drawAttr);

   glColor4fv(color.m_af);
   pfont->renderC(str, _labelPt);
}

//============================================================================
//============================================================================
void GeoPoly::drawDebug(DrawData *pdd)
{
    box3d box;
    computeBox(pdd, &box);

    float c[] { 1, 0, 0, 1 };
    glColor4fv(c);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);

    glVertex2d(box.vmin.x, box.vmin.y);
    glVertex2d(box.vmax.x, box.vmin.y);
    glVertex2d(box.vmax.x, box.vmax.y);
    glVertex2d(box.vmin.x, box.vmax.y);

    glEnd();
}

//============================================================================
//============================================================================
void GeoPoly::computeBox(DrawData *pdd, box3d *pbox)
{
    GeoObj::computeBox(pdd, pbox);

    for (unsigned int i=0; i<size(); i++)
    {
        Point2d pt = at(i);
        pbox->updateBox(pt.dX, pt.dY, 0);
    }

    if (!pdd) return;

    if (!DrawAttr::drawLabels(pdd->_drawAttr, _drawAttr)) return;
    std::string *pfeature = DrawAttr::feature(pdd->_drawAttr, _drawAttr);
    if (!pfeature) return;

    GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
    if (!pfont) return;

    // TODO: we need the text box here don't we


}

//============================================================================
//============================================================================
void GeoPoly::computeBoxNoLabel(box3d *pbox) const
{
    //GeoObj::computeBox(pdd, pbox);

    for (unsigned int i=0; i<size(); i++)
    {
        Point2d pt = at(i);
        pbox->updateBox(pt.dX, pt.dY, 0);
    }
}

//============================================================================
//============================================================================
void GeoPoly::initLabels(DrawData *pdd)
{
    GeoObj::initLabels(pdd);

    _labelPt = findLabelPt(pdd);
}

//============================================================================
//============================================================================
void GeoPoly::push_back(const Point2d &pt)
{
    _pts.push_back(pt);
}


//============================================================================
//============================================================================
void GeoPoly::scale(double x, double y, double z)
{
    GeoObj::scale(x,y,z);

    for (int i=0; i<_pts.size(); i++)
    {
        Point2d *pt = &_pts[i];
        pt->dX *= x;
        pt->dY *= y;
    }
}

//============================================================================
//============================================================================
void GeoPoly::translate(double x, double y, double z)
{
    GeoObj::translate(x,y,z);

    for (int i=0; i<_pts.size(); i++)
    {
        Point2d *pt = &_pts[i];
        pt->dX += x;
        pt->dY += y;
    }
}

//============================================================================
//============================================================================
void GeoPoly::moveFromTo(const vec3d &fr, const vec3d &to)
{
    GeoObj::moveFromTo(fr, to);

    for (int i=0; i<_pts.size(); i++)
    {
        Point2d *pt = &_pts[i];
        pt->dX -= fr.x;
        pt->dY -= fr.y;
        pt->dX += to.x;
        pt->dY += to.y;
    }
}

//============================================================================
//============================================================================
Point2d GeoPoly::at(int i) const
{
    if (i< 0 || i>= size())
        return Point2d();
    else
        return _pts[i];
}

//============================================================================
//============================================================================
void GeoPoly::insertField(const char *name, int v)
{
    _fieldsInt[std::string(name)] = v;
}

//============================================================================
//============================================================================
void GeoPoly::insertField(const char *name, double v)
{
    _fieldsDbl[std::string(name)] = v;
}

//============================================================================
//============================================================================
void GeoPoly::insertField(const char *name, const char *v)
{
    _fieldsStr[std::string(name)] = std::string(v);
}

//============================================================================
//============================================================================
bool GeoPoly::haveFeature(DrawData *pdd, std::string *value)
{
    if (!getDrawFeature(pdd, value)) return false;
    return true;
}

//============================================================================
//============================================================================
bool GeoPoly::getFeatureValue(const char *feature, std::string *value)
{
    return getFeatureValue(std::string(feature), value);
}

//============================================================================
//============================================================================
bool GeoPoly::getFeatureValue(const std::string &feature, std::string *value)
{
    char format[64];
    MapFieldsDbl::iterator itd = _fieldsDbl.find(feature);
    if (itd != _fieldsDbl.end())
    {
        sprintf(format, "%.2f", itd->second);
        *value = format;
        return true;
    }

    MapFieldsInt::iterator iti = _fieldsInt.find(feature);
    if (iti != _fieldsInt.end())
    {
        sprintf(format, "%.2i", iti->second);
        *value = format;
        return true;
    }

    MapFieldsStr::iterator its = _fieldsStr.find(feature);
    if (its != _fieldsStr.end())
    {
        *value = its->second;
        return true;
    }

    return false;
}

//============================================================================
//============================================================================
bool GeoPoly::getFeatureValue(const std::string &feature, double *value)
{
    //char format[64];
    MapFieldsDbl::iterator itd = _fieldsDbl.find(feature);
    if (itd != _fieldsDbl.end())
    {
        *value = itd->second;
        return true;
    }

    MapFieldsInt::iterator iti = _fieldsInt.find(feature);
    if (iti != _fieldsInt.end())
    {
        *value = (double)iti->second;
        return true;
    }

    MapFieldsStr::iterator its = _fieldsStr.find(feature);
    if (its != _fieldsStr.end())
    {
        *value = atof(its->second.c_str());
        return true;
    }

    return false;
}

//============================================================================
//============================================================================
bool GeoPoly::getDrawFeature(DrawData *pdd, std::string *str)
{
    char s[256];
    if (!getDrawFeature(pdd, s)) return false;
    *str = s;
    return true;
}

//============================================================================
// todo: convert this to format and stl string
//============================================================================
bool GeoPoly::getDrawFeature(DrawData *pdd, char *str)
{
    //if (!DrawAttr::drawLabels(pdd->_drawAttr, _drawAttr)) return false;
    std::string *pfeature = DrawAttr::feature(pdd->_drawAttr, _drawAttr);
    if (!pfeature) return false;

    //GlText *pfont = DrawAttr::font(pdd->_drawAttr, _drawAttr);
    //if (!pfont) return false;

    MapFieldsDbl::iterator itd = _fieldsDbl.find(*pfeature);
    if (itd != _fieldsDbl.end())
    {
        sprintf(str, "%.2f", itd->second);
        return true;
    }

    MapFieldsInt::iterator iti = _fieldsInt.find(*pfeature);
    if (iti != _fieldsInt.end())
    {
        sprintf(str, "%.2i", iti->second);
        return true;
    }

    MapFieldsStr::iterator its = _fieldsStr.find(*pfeature);
    if (its != _fieldsStr.end())
    {
        sprintf(str, "%s", its->second.c_str());
        return true;
    }

    return false;
}

//============================================================================
//============================================================================
Point2d GeoPoly::findLabelPt(DrawData *pdd)
{
    const char *func = "GeoPoly::findLabelPt() - ";

    box3d box;
    computeBoxNoLabel(&box);

    if (!DrawAttr::drawLabels(pdd->_drawAttr, _drawAttr))
    {
        return box.getCenter().get2d();
    }

    char str[64];
    getDrawFeature(pdd, str);
    if (!strcmp(str, "16B"))
    {
        int idebug = 1;
    }

    vec3d c = box.getCenter();
    Point2d boxcLeft(box.vmin.x, c.y);
    Point2d boxcRight(box.vmax.x, c.y);

    Point2d centroid = findCentroid();
    Point2d centLeft(box.vmin.x, centroid.dY);
    Point2d centRight(box.vmax.x, centroid.dY);

    Point2d labelPt;
    if (!findLabelPointFromIsects(pdd, centLeft, centRight, &labelPt))
    {
        if (!findLabelPointFromIsects(pdd, boxcLeft, boxcRight, &labelPt))
        {
            labelPt = Point2d(c.x, c.y);
        }
    }

    return labelPt;

    /*
    //if (loc & DrawAttr::Middle)

    // teting just compute middle
    vec3d c = box.GetCenter();

    // create horizontal segment
    Point2d left(box.vmin.x, c.y);
    Point2d right(box.vmax.x, c.y);
    ListPt2d isects;

    findIntersections(left, right, &isects, false, true);

    Point2d pt1, pt2;
    if (isects.size() < 2)
    {
        LogError("%s Unexpected intersection count %d, should be 2", func, isects.size());
        return Point2d(c.x, c.y);
    }
    else if (isects.size() > 2)
    {
        LogTrace("%s More than 2 intersections, find closest 2 points", func, isects.size());

        // todo: find mid points of all combinations and make sure result is in the poly

        double dis = 99999999999;
        pt1 = isects[0];
        for (unsigned int i=1; i<isects.size(); i++)
        {
            Point2d vdis = isects[i] - pt1;
            if (vdis.dLen() < dis) pt2 = isects[i];
        }
    }
    else
    {
         pt1 = isects[0];
         pt2 = isects[1];
    }

    Point2d vmove = pt2 - pt1;
    double len = vmove.dLen();
    vmove.vNormalize();
    vmove *= len/2;

    Point2d pos = pt1 + vmove;

    // TODO: get override attribute here if there is one
    if (pdd->_drawAttr && pdd->_drawAttr->_useLabelPosOffset)
    {
        pos.dX += pdd->_drawAttr->_labelOfs.dX;
        pos.dY += pdd->_drawAttr->_labelOfs.dY;
     }

    return pos;
    */
}

//============================================================================
//============================================================================
bool GeoPoly::findLabelPointFromIsects(DrawData *pdd, const Point2d &left, const Point2d &right, Point2d *result)
{
    const char *func = "GeoPoly::findLabelPtFromIsects() - ";

    ListPt2d isects;
    findIntersections(left, right, &isects, false, true);

    Point2d pt1, pt2;

    if (isects.size() < 2)
    {
        LogError("%s Unexpected intersection count %d, should be 2", func, isects.size());
        return false;
    }
    else if (isects.size() > 2)
    {
        LogTrace("%s More than 2 intersections, find closest 2 points", func, isects.size());

        // todo: find mid points of all combinations and make sure result is in the poly

        double dis = 99999999999;
        pt1 = isects[0];
        for (unsigned int i = 1; i<isects.size(); i++)
        {
            Point2d vdis = isects[i] - pt1;
            if (vdis.dLen() < dis) pt2 = isects[i];
        }
    }
    else
    {
        pt1 = isects[0];
        pt2 = isects[1];
    }

    Point2d vmove = pt2 - pt1;
    double len = vmove.dLen();
    vmove.vNormalize();
    vmove *= len / 2;

    Point2d pos = pt1 + vmove;
    // TODO: get override attribute here if there is one
    if (pdd->_drawAttr && pdd->_drawAttr->_useLabelPosOffset)
    {
        pos.dX += pdd->_drawAttr->_labelOfs.dX;
        pos.dY += pdd->_drawAttr->_labelOfs.dY;
    }

    *result = pos;
    return true;
}

//============================================================================
//============================================================================
int GeoPoly::findIntersections(const Point2d &sp1, const Point2d &sp2, ListPt2d *plist, bool ptOnSeg, bool ptOnPly)
{
    if (_pts.size() < 2 ) return 0;

    UtlLine2d::SIntersect sti;
    SLineSeg stSeg(sp1, sp2);
    SLineSeg stSegPly;

    Point2d pt1, pt2;


    pt1 = _pts[0];
    int count = 0;
    for (unsigned int i=1; i<_pts.size(); i++)
    {
        pt2 = _pts[i];

        stSegPly.pt1 = pt1;
        stSegPly.pt2 = pt2;
        pt1 = pt2;

        if (!UtlLine2d::intersect(stSeg, stSegPly, &sti, ptOnSeg, ptOnPly)) continue;
        plist->push_back(sti.ipt);
        count++;
    }

    // last to first
    if (_pts.size() > 2)
    {
        if (UtlLine2d::intersect(stSeg, SLineSeg(_pts[_pts.size()-1], _pts[0]), &sti, ptOnSeg, ptOnPly))
        {
            plist->push_back(sti.ipt);
            count++;
        }

    }

    return count;
}


//============================================================================
//  Returns twice the signed area of the triangle determined by a,b,c,
//  positive if a,b,c are oriented ccw, and negative if cw.
//============================================================================
double area2( const Point2d &a, const Point2d &b, const Point2d &c )
{
    return (b.dX - a.dX) * (c.dY - a.dY) - (c.dX - a.dX) * (b.dY - a.dY);
}

//============================================================================
// Returns three times the centroid.  The factor of 3 is
// left in to permit division to be avoided until later.
//============================================================================
void centroid3( const Point2d &p1, const Point2d &p2, const Point2d &p3, Point2d &c )
{
    c.dX = p1.dX + p2.dX + p3.dX;
    c.dY = p1.dY + p2.dY + p3.dY;
}

//============================================================================
//============================================================================
Point2d GeoPoly::findCentroid()
{
    int     i;
    double  A2, Areasum2 = 0;        /* Partial area sum */
    Point2d Cent3;

    Point2d CG;
    CG.dX = 0;
    CG.dY = 0;
    for (i = 1; i < size()-1; i++)
    {
        centroid3( _pts[0], _pts[i], _pts[i+1], Cent3 );
        A2 =  area2( _pts[0], _pts[i], _pts[i+1]);
        CG.dX += A2 * Cent3.dX;
        CG.dY += A2 * Cent3.dY;
        Areasum2 += A2;
    }

    CG.dX /= 3 * Areasum2;
    CG.dY /= 3 * Areasum2;


    return CG;
}
