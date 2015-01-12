#include "utlgeom2d.h"


//============================================================================
//============================================================================
bool UtlGeom2d::ptInPoly(const Point2d &pt, const Point2d *pts, int numpts)
{
    if (numpts < 3) return false;

    int i, j, c = 0;
    for (i = 0, j = numpts-1; i < numpts; j = i++)
    {
        if ((((pts[i].dY<=pt.dY) && (pt.dY<pts[j].dY)) ||
            ((pts[j].dY<=pt.dY) && (pt.dY<pts[i].dY))) &&
            (pt.dX < (pts[j].dX - pts[i].dX) * (pt.dY - pts[i].dY) / (pts[j].dY - pts[i].dY) + pts[i].dX))

        c = !c;
    }

    return c;
}
