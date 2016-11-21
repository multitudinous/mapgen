#ifndef UTLGEOM2D_H
#define UTLGEOM2D_H

#include "point2d.h"
#include "rect2d.h"

class UtlGeom2d
{
public:
    UtlGeom2d() {}

    static bool ptInPoly(const Point2d &pt, const Point2d *pts, int numpts);

    static void fit(double srcW, double srcH, const Rect2d &rcDstTot, Rect2d *rcDst);

	static bool isCCW(const ListPt2d &pts, bool loop = true);
};

#endif
