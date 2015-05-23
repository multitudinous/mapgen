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

    static void drawLineLoop(const ListPt2d &pts);

    static void drawMask(DrawData *pdd, const GlObjList* maskList, const ListPt2d &regionPtList);
    static void drawMask(DrawData *pdd, GlObj *maskColoring, const ListPt2d &regionPtList);

    static void drawQuad(double l, double r, double t, double b, double z = 0);
    static void drawQuadLine(double l, double r, double t, double b, double z = 0);
    static void drawQuad(GLenum mode, double l, double r, double t, double b, double z = 0);

	//static void drawPolyConcaveVN(GLUtesselator *hTess, const std::vector<vec3d> &ptlist, const Vector3 &clN);
    static void drawPolyConcaveVN(GLUtesselator *hTess, const GeoPoly *poly);
    static void drawPolyConcaveVN(GLUtesselator *hTess, const ListPt2d &ptlist);

protected:
	static void gluCallbackTessalateVertVN3F(void *pvVertInfoVN3);
	static void gluCallbackBeginPoly(GLenum eWhich);
};

#endif
