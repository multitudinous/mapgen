/**
 * Copyright 2007 
 * Code Hammer Technologies, LLC
 * www.code-hammer.com
 *
 */
#ifndef POINT2D_H
#define POINT2D_H

#include "sysdefs.h"

//class CPoint;

// +
// Start Point2d
class Point2d
{
	public:
        Point2d();
        Point2d(double dXPt, double dYPt);
        Point2d(const Point2d &clPt);
        //Point2d(const CPoint &clPt);
        ~Point2d();

    void vNormalize();
    void vRot(double dRad);
    void vTrans(const Point2d &clPt);
    double dLen() const;
    double dGetAng(const Point2d &clPt, bool bNormalize=true);
	double dPolarAngle(bool bNormalize=true) const;
    void vGetPolarPos(double dRad, double dLen);
    int iGetQuadrant() const;
    void vWinToCar(double dH);
    void vCarToWin(double dH);

    void vGet(double ad2[2]);
    void vSet(double ad2[2]);

        Point2d& operator= (const Point2d &clPt);
    //Point2d& operator= (const CPoint &clPt);
    Point2d& operator- ();
    Point2d  operator* (double d) const;
    double   operator* (const Point2d &clPt) const; // dot product
    Point2d  operator+ (const Point2d &clPt) const;
    Point2d  operator- (const Point2d &clPt) const;
    Point2d operator/ (const double d) const;
    Point2d& operator+= (const Point2d &clPt);
     Point2d& operator-= (const Point2d &clPt);
    Point2d& operator*= (double d);
    //operator CPoint() const;
    friend bool operator== (const Point2d &clPt1, const Point2d &clPt2);
    friend bool operator!= (const Point2d &clPt1, const Point2d &clPt2);
    friend bool bPtEquality(const Point2d &clPt1, const Point2d &clPt2, double dEpsilon=.00001); // equality but allows for epsilon adjustment

  public:
		double dX;
		double dY;
};
// End Point2d
// -

struct SLineSeg
{
  Point2d pt1;
  Point2d pt2;

  SLineSeg() {}

  SLineSeg(const Point2d &p1, const Point2d &p2)
  {
      pt1 = p1;
      pt2 = p2;
  }
};

typedef std::vector<Point2d> ListPt2d;

#endif
