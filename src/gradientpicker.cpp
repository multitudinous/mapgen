#include "gradientpicker.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRect>

//============================================================================
//============================================================================
GradientPicker::GradientPicker()
{
    _img.reset(new QImage ( 10, 100, QImage::Format_ARGB32 ));
}

//============================================================================
//============================================================================
GradientPicker::GradientPicker(QColor min, QColor mid, QColor max)
{
    _img.reset(new QImage ( 10, 100, QImage::Format_ARGB32 ));
    create(min, mid, max);
}

//============================================================================
//============================================================================
GradientPicker::GradientPicker(QColor min, QColor max)
{
    _img.reset(new QImage ( 10, 100, QImage::Format_ARGB32 ));
    create(min, max);
}

//============================================================================
//============================================================================
void GradientPicker::create(QColor min, QColor mid, QColor max)
{
    QRect rc(0,0,10, 100);
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
void GradientPicker::create(QColor min, QColor max)
{
    QRect rc(0,0,10, 100);
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
QColor GradientPicker::pick(float percent) const
{
    float x = (int)(_img->width() / 2.0);
    float y = int (float(_img->height()) * percent);

    if (y < 0)
    {
        y = 0;
    }
    if (y >= _img->height())
    {
        y = _img->height() - 1;
    }

    return QColor(_img->pixel(x,y));
}

//============================================================================
//============================================================================
QColor GradientPicker::getMin() const
{
    return pick(0);
}

//============================================================================
//============================================================================
QColor GradientPicker::getMid() const
{
    return pick(.5);
}

//============================================================================
//============================================================================
QColor GradientPicker::getMax() const
{
    return pick(1);
}
