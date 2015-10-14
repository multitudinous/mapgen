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

    if (c == 0)
    {
        return false;
    }

    return true;
}

//============================================================================
//============================================================================
void UtlGeom2d::fit(double srcW, double srcH, const Rect2d &rcDstTot, Rect2d *rcDst)
{
    double scale = (rcDstTot.width() / srcW);
    if (srcH * scale > rcDstTot.height())
    {
        scale = rcDstTot.height() / srcH;
    }

    double resultW = scale * srcW;
    double resultH = scale * srcH;

    // center the result rect within the destination rect

    // center width
    double difW = (rcDstTot.width() - resultW) / 2.0;
    rcDst->left(rcDstTot.left() + difW);
    rcDst->right(rcDst->left() + resultW);
    
    // center height
    double difH = (rcDstTot.height() - resultH) / 2.0;
    rcDst->bottom(rcDstTot.bottom() + difH);
    rcDst->top(rcDst->bottom() + resultH);
}