#include "colorramp.h"

//============================================================================
//============================================================================
ColorRamp::ColorRamp() :
    _buckets(0),
    _minv(0),
    _maxv(0)
{

}


//============================================================================
//============================================================================
Rgbf ColorRamp::getFromPer(float per)
{
    validate();

    return getRgb(_picker->pick(per));
}

//============================================================================
//============================================================================
Rgbf ColorRamp::getFromVal(double v)
{
    validate();

    // get the correct bucket
    double dist = _maxv - _minv;
    double disa = v - _minv;
    double perd = disa / dist;

    double bucketd = (double)_buckets * perd;
    int bucket = (int)bucketd; // truncate it

    float perb = (float)bucket / (float)_buckets; // get the same color everytime for every bucket


    return getRgb(_picker->pick(perb));
}

//============================================================================
//============================================================================
bool ColorRamp::hasBuckets()
{
    if (_buckets > 0) return true;

    return false;
}

//============================================================================
//============================================================================
void ColorRamp::validate()
{
    if (!_picker)
    {
        LogError("ColorRamp::validate - gradientpicker not created, creating a default one.");
        _picker.reset(new GradientPicker(QColor("#55aa00"), QColor("#ffff00"), QColor("#ff0000")));
    }
}

//============================================================================
//============================================================================
Rgbf ColorRamp::getRgb(const QColor &c)
{
    float r = c.redF();
    float g = c.greenF();
    float b = c.blueF();
    float a = c.alphaF();

    return Rgbf(r, g, b, a);
}