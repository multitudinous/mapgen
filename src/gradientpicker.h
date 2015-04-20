#ifndef GRADIENTPICKER_H
#define GRADIENTPICKER_H

#include <memory>
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
    std::shared_ptr<QImage> _img;
};

typedef std::shared_ptr< GradientPicker > PGradientPicker;
#endif
