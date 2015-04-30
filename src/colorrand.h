#ifndef COLORRAND_H
#define COLORRAND_H

#include "rgbf.h"

class ColorRand
{
public:
    ColorRand();
    ColorRand(double s, double v); // will generate starting h value randomly
    ColorRand(double hStart, double s, double v);
    
    void setHSV(double h, double s, double v);
    void setH(double h);
    void setS(double s);
    void setV(double v);

    Rgbf getNext();

protected:
    double _h;
    double _s;
    double _v;
};

typedef shared_ptr<ColorRand> PColorRand;
#endif