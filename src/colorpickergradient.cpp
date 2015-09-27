#include "colorpickergradient.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRect>

//============================================================================
//============================================================================
ColorPickerGradient::ColorPickerGradient()
{
    _img.reset(new QImage(10, 100, QImage::Format_ARGB32));
}

//============================================================================
//============================================================================
ColorPickerGradient::ColorPickerGradient(QColor min, QColor mid, QColor max)
{
    _img.reset(new QImage(10, 100, QImage::Format_ARGB32));
    create(min, mid, max);
}

//============================================================================
//============================================================================
ColorPickerGradient::ColorPickerGradient(QColor min, QColor max)
{
    _img.reset(new QImage(10, 100, QImage::Format_ARGB32));
    create(min, max);
}

//============================================================================
//============================================================================
void ColorPickerGradient::create(QColor min, QColor mid, QColor max)
{
    QRect rc(0, 0, 10, 100);
    QLinearGradient gradient(rc.topLeft(), rc.bottomRight());
    gradient.setColorAt(0.0, min);
    gradient.setColorAt(0.5, mid);
    gradient.setColorAt(1.0, max);


    QPainter painter(_img.get());

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rc, gradient);
    painter.end();
}

//============================================================================
//============================================================================
void ColorPickerGradient::create(QColor min, QColor max)
{
    QRect rc(0, 0, 10, 100);
    QLinearGradient gradient(rc.topLeft(), rc.bottomRight());
    gradient.setColorAt(0.0, min);
    gradient.setColorAt(1.0, max);


    QPainter painter(_img.get());

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rc, gradient);
    painter.end();
}

//============================================================================
//============================================================================
QColor ColorPickerGradient::pickPrefered(float percent, double value) const
{
    return pickByPercent(percent);
}

//============================================================================
//============================================================================
QColor ColorPickerGradient::pickByPercent(float percent) const
{
    float x = (int)(_img->width() / 2.0);
    float y = int(float(_img->height()) * percent);

    if (y < 0)
    {
        y = 0;
    }
    if (y >= _img->height())
    {
        y = _img->height() - 1;
    }

    return QColor(_img->pixel(x, y));
}

//============================================================================
// not supported
//============================================================================
QColor ColorPickerGradient::pickByValue(double value) const
{
    return QColor(0, 0, 0, 255);
}

//============================================================================
//============================================================================
QColor ColorPickerGradient::getMin() const
{
    return pickByPercent(0);
}

//============================================================================
//============================================================================
QColor ColorPickerGradient::getMid() const
{
    return pickByPercent(.5f);
}

//============================================================================
//============================================================================
QColor ColorPickerGradient::getMax() const
{
    return pickByPercent(1);
}
