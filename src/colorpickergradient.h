#ifndef COLORPICKERGRADIENT_H
#define COLORPICKERGRADIENT_H

#include <memory>
#include <QImage>
#include <QColor>
#include "colorpicker.h"

class ColorPickerGradient : public ColorPicker
{
public:
    ColorPickerGradient();
    ColorPickerGradient(QColor min, QColor max);
    ColorPickerGradient(QColor min, QColor mid, QColor max);

    void create(QColor min, QColor mid, QColor max);
    void create(QColor min, QColor max);

    virtual QColor pickPrefered(float percent, double value, bool validation);
    virtual QColor pickPrefered(float percent, double value) const;
    virtual QColor pickByPercent(float percent) const;
    virtual QColor pickByValue(double value) const;

    virtual QColor getMin() const;
    virtual QColor getMid() const;
    virtual QColor getMax() const;

    virtual bool hasPickPercent() const { return true; }
    virtual bool hasPickValue() const { return false; }

    virtual bool hasImg() { return true; }
    virtual bool saveImg(const char *path);
    virtual bool saveImgVal(const char *path);

    void logPercent(double per); //for validation

protected:
    std::shared_ptr<QImage> _img;

    std::map<int, int> _percentMap; // for validation
};

typedef std::shared_ptr< ColorPickerGradient > PColorPickerGradient;
#endif
