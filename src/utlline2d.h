#ifndef UTLLINE2D_H
#define UTLLINE2D_H

#include "sysdefs.h"
#include "point2d.h"

class UtlLine2d
{
public:
    enum
    {
        I_LINE_NORMAL,
        I_LINE_HORIZONTAL,
        I_LINE_VERTICAL
    };

    struct SIntersect
    {
      Point2d ipt;   // the intersection point
      bool collinear; // are the lines collinear (lie on top of each other)
      bool parallel;  // are the lines parallel
      bool iOnSegs;   // intersection exists and is within both segments
      bool iOnSeg1;   // intersection exists and is within segment 1
      bool iOnSeg2;   // intersection exists and is within segment 2
      bool iOnLine;   // intersection exists but in not within both segments
      double seg1Pos; // parametric value which determines the position on the line
      double seg2Pos; // parametric value which determines the position on the line

      SIntersect()
      {
        ipt       = Point2d(0,0);
        collinear  = FALSE;
        parallel   = FALSE;
        iOnSegs    = FALSE;
        iOnSeg1    = FALSE;
        iOnSeg2    = FALSE;
        iOnLine    = FALSE;
        seg1Pos    = 0;
        seg2Pos    = 0;
      }
    };

    static const double dEpsilon;

public:
    UtlLine2d() {}

    static bool intersect(const SLineSeg &stSeg1, const SLineSeg &stSeg2, SIntersect *pstInter);
    static bool intersect(const SLineSeg &stSeg1, const SLineSeg &stSeg2, SIntersect *pstInter, bool onSeg1, bool onSeg2);
};

#endif
