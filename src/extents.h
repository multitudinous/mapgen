#ifndef EXTENTS_H
#define EXTENTS_H

#include <string>
#include "point2d.h"

struct Extents
{
	double l,r,t,b;

	Extents()
	{
		l=0;
		r=0;
		t=0;
		b=0;
	}

    Extents(const Extents &ext)
    {
        l = ext.l;
        r = ext.r;
        t = ext.t;
        b = ext.b;
    }

	Extents(double left, double top, double right, double bottom)
	{
		l=left;
		r=right;
		t=top;
		b=bottom;
	}

    double cx() const { return l + (r-l)/2.0; }
    double cy() const { return b + (t-b)/2.0; }
    Point2d center() const { return Point2d(cx(), cy()); }
    double width() const { return r-l; }
    double height() const { return t-b; }
    double area() const { return width() * height(); }
    bool isZero() const { if (fabs(width()) < .001 && fabs(height()) < .001) return true; return false; }


    void move(const Point2d &mv)
    {
        Point2d c = center();
        c += mv;
        setFromCenter(c.dX, c.dY, width(), height());
    }

    void setFromCenter(double centerx, double centery, double width, double height)
    {
        l = centerx - width/2.0;
        r = centerx + width/2.0;
        b = centery - height/2.0;
        t = centery + height/2.0;
    }

    void zoom(double per)
    {
        double neww = width() * per;
        double newh = height() * per;
        double centerx = cx();
        double centery = cy();
        setFromCenter(centerx, centery, neww, newh);
    }

    void makeSquare()
    {
        double w = width();
        double h = height();

        double grow  = w - h;

        if (grow < 0)
            r += grow; // grow horizontal to match height size
        else
            t += grow; // grow vertical to match width size

        /*
        double grow  = (w - h)/2.0;

        if (grow < 0)
            expandWidth(grow);
        else
            expandHeight(grow);
        */
    }

    void expand(double d)
	{
        expandWidth(d);
        expandHeight(d);
	}

    void expandWidth(double d)
    {
        l -= d;
        r += d;
     }

    void expandHeight(double d)
    {
        t += d;
        b -= d;
     }

	std::string asStr() const
	{
		char str[128];
		sprintf(str, "l: %.6f, r: %.6f, t: %.6f, b: %.6f", l, r, t, b);
		return std::string(str);
	}

	Extents operator* (double d) const
	{
		Extents ext;
		ext.l = l*d;
		ext.r = r*d;
		ext.t = t*d;
		ext.b = b*d;

		return ext;
	}

    Extents& operator= (const Extents &ext)
    {
        if (this == &ext)
            return *this;

        l = ext.l;
        r = ext.r;
        t = ext.t;
        b = ext.b;

       return *this;
    }
};

#endif
