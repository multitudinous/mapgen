#include "utlline2d.h"
#include "utlmisc.h"

//======================================================================
//======================================================================
bool UtlLine2d::intersect(const SLineSeg &stSeg1, const SLineSeg &stSeg2, SIntersect *pstInter)
{
  pstInter->collinear  = false;
  pstInter->parallel   = false;
  pstInter->iOnLine    = false;
  pstInter->iOnSegs    = false;
  pstInter->iOnSeg1    = false;
  pstInter->iOnSeg2    = false;
  double dDen, dNum1, dNum2, dR, dS;
  dDen  = (stSeg1.pt2.dX - stSeg1.pt1.dX)*(stSeg2.pt2.dY - stSeg2.pt1.dY) - (stSeg1.pt2.dY - stSeg1.pt1.dY)*(stSeg2.pt2.dX - stSeg2.pt1.dX);
  dNum1 = (stSeg1.pt1.dY - stSeg2.pt1.dY)*(stSeg2.pt2.dX - stSeg2.pt1.dX) - (stSeg1.pt1.dX - stSeg2.pt1.dX)*(stSeg2.pt2.dY - stSeg2.pt1.dY);
  dNum2 = (stSeg1.pt1.dY - stSeg2.pt1.dY)*(stSeg1.pt2.dX - stSeg1.pt1.dX) - (stSeg1.pt1.dX - stSeg2.pt1.dX)*(stSeg1.pt2.dY - stSeg1.pt1.dY);
  if (UtlMisc::equality(dDen, 0, .0001))
  {
    pstInter->parallel = true;
    if (UtlMisc::equality(dNum1, 0, .0001))
      pstInter->collinear = true;
    return false;
  }
  dR = dNum1/dDen;
  dS = dNum2/dDen;
  pstInter->ipt.dX = stSeg1.pt1.dX + dR*(stSeg1.pt2.dX - stSeg1.pt1.dX);
  pstInter->ipt.dY = stSeg1.pt1.dY + dR*(stSeg1.pt2.dY - stSeg1.pt1.dY);
  pstInter->iOnLine = true;
  if (dR >= 0 && dR <= 1)
    pstInter->iOnSeg1 = true;
  if (dS >= 0 && dS <= 1)
    pstInter->iOnSeg2 = true;

  if (pstInter->iOnSeg1 && pstInter->iOnSeg2)
    pstInter->iOnSegs = true;

  pstInter->seg1Pos = dR;
  pstInter->seg2Pos = dS;

  return true;
}

//======================================================================
//======================================================================
bool UtlLine2d::intersect(const SLineSeg &stSeg1, const SLineSeg &stSeg2, SIntersect *psti, bool ptOnSeg1, bool ptOnSeg2)
{
    if (!intersect(stSeg1, stSeg2, psti)) return false;
    if (ptOnSeg1 && !psti->iOnSeg1) return false;
    if (ptOnSeg2 && !psti->iOnSeg2) return false;

    return true;
}
