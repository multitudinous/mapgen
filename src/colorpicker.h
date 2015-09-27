#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "rgbf.h"
#include <QColor>

class ColorPicker
{
public:
    ColorPicker();

    virtual QColor pickPrefered(float percent, double value) const = 0;
    virtual QColor pickByPercent(float percent) const = 0;
    virtual QColor pickByValue(double value) const = 0;

    virtual QColor getMin() const = 0;
    virtual QColor getMid() const = 0;
    virtual QColor getMax() const = 0;

    virtual bool hasPickPercent() const { return false; }
    virtual bool hasPickValue() const { return false; }

    virtual void begin(const char *id) {};
    virtual void end() {}
};
typedef shared_ptr<ColorPicker> PColorPicker;

#endif