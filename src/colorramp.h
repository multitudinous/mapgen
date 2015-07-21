#ifndef COLORRAMP_H
#define COLORRAMP_H

#include "rgbf.h"
#include "gradientpicker.h"

class ColorRamp
{
public:
    ColorRamp();

    Rgbf getFromPer(float per);
    Rgbf getFromVal(double v);

    bool hasBuckets();

    static Rgbf getRgb(const QColor &c);

protected:
    void validate();

public:
    std::string _name;
    PGradientPicker _picker;

    int _buckets;
    double _minv; // bucket range min
    double _maxv; // bucket range max
};
typedef shared_ptr<ColorRamp> PColorRamp;

#endif