#ifndef GLDRAW_H
#define GLDRAW_H

#include "vector3.h"
#include "point2d.h"

#define CALLBACK
class GeoPoly;

class Gldraw
{
public:
	Gldraw() {};

	//static void drawPolyConcaveVN(GLUtesselator *hTess, const std::vector<vec3d> &ptlist, const Vector3 &clN);
    static void drawPolyConcaveVN(GLUtesselator *hTess, const GeoPoly *poly);
    static void drawPolyConcaveVN(GLUtesselator *hTess, const std::vector<Point2d> &ptlist);

protected:
	static void CALLBACK gluCallbackTessalateVertVN3F(void *pvVertInfoVN3);
	static void CALLBACK gluCallbackBeginPoly(GLenum eWhich);
};

#endif
