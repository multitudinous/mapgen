#include "rect2d.h"
#include <math.h>

//============================================================================
//============================================================================
Rect2d::Rect2d() :
_l(0),
_r(0),
_t(0),
_b(0)
{
}

//============================================================================
//============================================================================
Rect2d::Rect2d(double l, double r, double t, double b)
{
    set(l, r, t, b);
}

//============================================================================
//============================================================================
double Rect2d::width() const
{
    return fabs(_r - _l);
}

//============================================================================
//============================================================================
double Rect2d::height() const
{
    return fabs(_t - _b);
}

//============================================================================
//============================================================================
void Rect2d::normalize(const Rect2d &oldBounds, const Rect2d &newBounds)
{
    double oldW = oldBounds.width();
    double oldH = oldBounds.height();
    double newW = newBounds.width();
    double newH = newBounds.height();

    double perL = (_l - oldBounds.l()) / oldW;
    double perR = (_r - oldBounds.l()) / oldW;
    double perB = (_b - oldBounds.b()) / oldH;
    double perT = (_t - oldBounds.b()) / oldH;

    _l = newBounds.l() + perL * newW;
    _r = newBounds.l() + perR * newW;
    _b = newBounds.b() + perB * newH;
    _t = newBounds.b() + perT * newH;
}

//============================================================================
//============================================================================
void Rect2d::set(double l, double r, double t, double b)
{
    _l = l;
    _r = r;
    _t = t;
    _b = b;
}

