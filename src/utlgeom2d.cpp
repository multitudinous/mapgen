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


//============================================================================
// Sum over the edges, (x2 − x1)(y2 + y1).
// If the result is positive the curve is clockwise, if it's negative the curve is counter-clockwise. 
// (The result is twice the enclosed area, with a +/- convention.)
//============================================================================
bool UtlGeom2d::isCCW(const ListPt2d &pts, bool loop)
{
	double sum = 0;

	const Point2d *pt1 = nullptr, *pt2 = nullptr;
	for (std::size_t i = 0; i < pts.size(); i++)
	{
		pt2 = &pts[i];
		if (!pt1)
		{
			pt1 = pt2;
			continue;
		}

		sum += (pt2->dX - pt1->dX) * (pt2->dY + pt1->dY);
		pt1 = pt2;
	}

	if (loop && pts.size() > 2)
	{
		pt1 = &pts[pts.size() - 1];
		pt2 = &pts[0];
		sum += (pt2->dX - pt1->dX) * (pt2->dY + pt1->dY);
	}

	if (sum < 0)
	{
		return true; // CCW
	}

	return false; // CW
}
