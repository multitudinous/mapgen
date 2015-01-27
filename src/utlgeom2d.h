#ifndef UTLGEOM2D_H
#define UTLGEOM2D_H

#include "point2d.h"

class UtlGeom2d
{
public:
    UtlGeom2d() {}

    static bool ptInPoly(const Point2d &pt, const Point2d *pts, int numpts);
};

#endif
