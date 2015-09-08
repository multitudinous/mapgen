/**
 * Copyright 2007
 * Code Hammer Technologies, LLC
 * www.code-hammer.com
 *
 */

// Includes
#pragma warning(disable: 4100)
#include "point2d.h"
#include "define.h"
#include <math.h>
#pragma warning(error: 4100)

// +
// *****************************************************************************
// *****************************************************************************
// -
Point2d::Point2d()
{
	dX = 0.0;
	dY = 0.0;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
Point2d::Point2d(double dXPt, double dYPt)
{
	dX = dXPt;
	dY = dYPt;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
Point2d::Point2d(const Point2d &clPt)
{
	dX = clPt.dX;
	dY = clPt.dY;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
/*
Point2d::Point2d(const CPoint &clPt)
{
	dX = (double)clPt.x;
	dY = (double)clPt.y;
}
*/

// +
// *****************************************************************************
// *****************************************************************************
// -
Point2d::~Point2d()
{
}

/**
 *
 *
 */
void Point2d::vGet(double ad2[2])
{
  ad2[0] = dX;
  ad2[1] = dY;
}

/**
 *
 *
 */
void Point2d::vSet(double ad2[2])
{
  dX = ad2[0];
  dY = ad2[1];
}

/**
 *
 */
void Point2d::vNormalize()
{
  double dLen = sqrt( (dX*dX + dY*dY) );
	if (fabs(dLen) <= .0001)
	{
		dX = 0;
		dY = 0;
	}
	else
	{
		dX /= dLen;
		dY /= dLen;
	}
}

/**
 *
 */
void Point2d::vRot(double dRad)
{
  Point2d clOldPt = *this;

  dX = clOldPt.dX*cos(dRad) - clOldPt.dY*sin(dRad);
  dY = clOldPt.dY*cos(dRad) + clOldPt.dX*sin(dRad);
}

/**
 *
 */
void Point2d::vTrans(const Point2d &clPt)
{
  dX -= clPt.dX;
  dY -= clPt.dY;
}

/**
 */
double Point2d::dLen() const
{
  return sqrt(dX*dX + dY*dY);
}

//=======================================================================
//=======================================================================
double Point2d::dGetAng(const Point2d &clPt, bool bNormalize)
{
  Point2d clV1 = *this, clV2 = clPt;

  if (bNormalize)
  {
    clV1.vNormalize();
    clV2.vNormalize();
  }

  double dDot = clV1*clV2;
	if (dDot > 1.0 || dDot < -1.0)
	{
		if (dDot > 1.0)
			dDot = 1.0;
		else
			dDot = -1.0;
	}

  return acos(dDot);
}

/**
 */
double Point2d::dPolarAngle(bool bNormalize) const
{
  double dAngle = 0.0;
  int iQuadrant = 1;

  Point2d clUnitZero(1,0);
  if (bNormalize)
  {
    Point2d clPt = *this;
    clPt.vNormalize();
    dAngle = acos(clPt*clUnitZero);

    iQuadrant = clPt.iGetQuadrant();
  }
  else
  {
    dAngle = acos(*this*clUnitZero);
    iQuadrant = iGetQuadrant();
  }
  switch (iQuadrant)
  {
    case 3:
    case 4:
      dAngle = Define::TWOPI - dAngle;
      break;
  }

  return dAngle;
}

/**
 */
void Point2d::vGetPolarPos(double dRad, double dLen)
{
  dX = dLen*cos(dRad);
  dY = dLen*sin(dRad);
}

/**
 */
int Point2d::iGetQuadrant() const
{
  if (dX < 0 && dY > 0)
    return 2;
  else if (dX < 0 && dY < 0)
    return 3;
  else if (dX > 0 && dY < 0)
    return 4;

  return 1;
}

/**
 *
 */
void Point2d::vWinToCar(double dH)
{
  dY = dH - dY;
}

/**
 *
 */
void Point2d::vCarToWin(double dH)
{
  dY = dH - dY;
}


Point2d Point2d::tangent(const Point2d &p1, const Point2d &p2)
{
    Point2d T(p2.x() - p1.x(), p2.y() - p1.y());

    // TODO: this should just be a normalize ?
    double norm = sqrt(T.x() * T.x() + T.y() * T.y());
    if (norm > 0)
    {
        T.dX /= norm;
        T.dY /= norm;
    }
    return T;
}


Point2d Point2d::ortho(const Point2d &p1, const Point2d &p2)
{
    Point2d T = tangent(p1, p2);
    return Point2d( -T.y(), T.x() );
}

int Point2d::intersection(const Point2d &P1, const Point2d &P2, const Point2d &P3, const Point2d &P4, Point2d *I)
{
    double epsilon = 1e-9;
    double mua, mub;
    double denom, numera, numerb;

    double x1 = P1.x(), y1 = P1.y();
    double x2 = P2.x(), y2 = P2.y();
    double x3 = P3.x(), y3 = P3.y();
    double x4 = P4.x(), y4 = P4.y();
    double *x = &(I->dX), *y = &(I->dY);

    denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    numera = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    numerb = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);

    /* Are the line coincident? */
    if ((fabs(numera) < epsilon) &&
        (fabs(numerb) < epsilon) &&
        (fabs(denom)  < epsilon))
    {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        // Segments intersect
        return 2;
    }

    /* Are the line parallel */
    if (fabs(denom) < epsilon)
    {
        *x = 0;
        *y = 0;
        return 0;
    }

    /* Is the intersection along the the segments */
    mua = numera / denom;
    mub = numerb / denom;
    *x = x1 + mua * (x2 - x1);
    *y = y1 + mua * (y2 - y1);
    if (mua < 0 || mua > 1 || mub < 0 || mub > 1)
    {
        // Lines intersect but no segments
        return 1;
    }
    // Segments intersect
    return 2;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
Point2d& Point2d::operator= (const Point2d &clPt)
{
	if (this != &clPt)
	{
		dX = clPt.dX;
		dY = clPt.dY;
	}

	return *this;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
/*
Point2d& Point2d::operator= (const CPoint &clPt)
{
	dX = (double)clPt.x;
	dY = (double)clPt.y;

	return *this;
}
*/

/**
 *
 */
Point2d& Point2d::operator- ()
{
  dX = -dX;
  dY = -dY;

  return *this;
}

/**
 *
 */
Point2d Point2d::operator* (double d) const
{
  Point2d clNewPt;
  clNewPt.dX = dX*d;
  clNewPt.dY = dY*d;

  return clNewPt;
}

/**
 *
 */
double Point2d::operator* (const Point2d &clPt) const
{
  return (dX*clPt.dX + dY*clPt.dY);
}

/**
 *
 */
Point2d Point2d::operator+ (const Point2d &clPt) const
{
  Point2d clNewPt;

  clNewPt.dX = dX+clPt.dX;
  clNewPt.dY = dY+clPt.dY;

  return clNewPt;
}

/**
 *
 */
Point2d Point2d::operator- (const Point2d &clPt) const
{
  Point2d clNewPt;

  clNewPt.dX = dX-clPt.dX;
  clNewPt.dY = dY-clPt.dY;

  return clNewPt;
}

/**
 *
 */
Point2d Point2d::operator/ (const double d) const
{
  Point2d clNewPt;

  clNewPt.dX = dX/d;
  clNewPt.dY = dY/d;

  return clNewPt;
}

//=======================================================================
//=======================================================================
Point2d& Point2d::operator+= (const Point2d &clPt)
{
	dX += clPt.dX;
	dY += clPt.dY;
	return *this;
}

//=======================================================================
//=======================================================================
Point2d& Point2d::operator-= (const Point2d &clPt)
{
	dX -= clPt.dX;
	dY -= clPt.dY;
	return *this;
}

//=======================================================================
//=======================================================================
Point2d& Point2d::operator*= (double d)
{
  dX *= d;
  dY *= d;
  return *this;
}

/**
 *
 */
/*
Point2d::operator CPoint() const
{
  return CPoint((long)dX, (long)dY);
}
*/
bool operator== (const Point2d &clPt1, const Point2d &clPt2)
{
  if (fabs(clPt1.dX - clPt2.dX) < .00001 &&
      fabs(clPt1.dY - clPt2.dY) < .00001)
    return true;

  return false;
}

bool operator!= (const Point2d &clPt1, const Point2d &clPt2)
{
  return !(clPt1 == clPt2);
}
