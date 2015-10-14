#ifndef RECT2D_H
#define RECT2D_H

class Rect2d
{
public:
    Rect2d();
    Rect2d(double l, double r, double t, double b);

    double width() const;
    double height() const;

    void normalize(const Rect2d &oldBounds, const Rect2d &newBounds);

    void set(double l, double r, double t, double b);

    void left(double v) { _l = v; }
    double left() const { return _l; }
    void l(double v) { _l = v; }
    double l() const { return _l; }

    void right(double v) { _r = v; }
    double right() const { return _r; }
    void r(double v) { _r = v; }
    double r() const { return _r; }

    void top(double v) { _t = v; }
    double top() const { return _t; }
    void t(double v) { _t = v; }
    double t() const { return _t; }

    void bottom(double v) { _b = v; }
    double bottom() const { return _b; }
    void b(double v) { _b = v; }
    double b() const { return _b; }


protected:
    double _l;
    double _r;
    double _t;
    double _b;
};

#endif