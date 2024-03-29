#ifndef GLDRAW_H
#define GLDRAW_H

#include "vector3.h"
#include "point2d.h"
#include "sysdefs.h"
#include "globj.h"

class GeoPoly;
class DrawData;

class Gldraw
{
public:
	Gldraw() {};

	static void drawPts(const ListPt2d &pts, float size, float r, float g, float b, float a=1);
    static void drawLine(double x1, double y1, double x2, double y2, //coordinates of the line
                         float w, //width/thickness of the line in pixel
                         float Cr, float Cg, float Cb); //RGB color components
    static void drawLine(const Point2d &pt1, const Point2d &pt2, float w, float r, float g, float b, float a=1);

    static void drawLineLoop(const ListPt2d &pts, float w, float r, float g, float b);
    static void drawLineLoopBasic(const ListPt2d &pts, float w, float r, float g, float b, float a=1);

    static void drawMask(DrawData *pdd, const GlObjList* maskList, const ListPt2d &regionPtList);
    static void drawMask(DrawData *pdd, GlObj *maskColoring, const ListPt2d &regionPtList);
	static void drawMaskOutInner(DrawData *pdd, const std::vector< const ListPt2d*> &outerList, const std::vector< const ListPt2d*> &innerList);

    static void drawQuad(double l, double r, double t, double b, double z = 0);
    static void drawQuadLine(double l, double r, double t, double b, double z = 0);
    static void drawQuad(GLenum mode, double l, double r, double t, double b, double z = 0);

	//static void drawPolyConcaveVN(GLUtesselator *hTess, const std::vector<vec3d> &ptlist, const Vector3 &clN);
    static void drawPolyConcaveVN(GLUtesselator *hTess, const GeoPoly *poly);
    static void drawPolyConcaveVN(GLUtesselator *hTess, const ListPt2d &ptlist);

protected:
	static void gluCallbackTessalateVertVN3F(void *pvVertInfoVN3);
	static void gluCallbackBeginPoly(GLenum eWhich);

    static PShaderProg _progSmoothLine;
};

#endif
