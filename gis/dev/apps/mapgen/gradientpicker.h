#ifndef GRADIENTPICKER_H
#define GRADIENTPICKER_H

#include "sysdefs.h"
#include <QImage>
#include <QColor>

class GradientPicker
{
public:
    GradientPicker();
    GradientPicker(QColor min, QColor max);
    GradientPicker(QColor min, QColor mid, QColor max);

    void create(QColor min, QColor mid, QColor max);
    void create(QColor min, QColor max);
    QColor pick(float percent);

protected:
    shared_ptr<QImage> _img;
};

typedef shared_ptr< GradientPicker > PGradientPicker;
#endif
