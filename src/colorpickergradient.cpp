#include "colorpickergradient.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRect>

//============================================================================
//============================================================================
ColorPickerGradient::ColorPickerGradient() :
    _img(new QImage(10, 100, QImage::Format_ARGB32))
{
    
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
QColor ColorPickerGradient::pickPrefered(float percent, double value, bool validation)
{
    if (validation)
    {
        logPercent(percent);
    }

    return pickPrefered(percent, value);
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

//============================================================================
//============================================================================
bool ColorPickerGradient::saveImg(const char *path)
{
    if (!_img) return false;
    return _img->save(path);
}

//============================================================================
//============================================================================
bool ColorPickerGradient::saveImgVal(const char *path)
{
    if (!_img) return false;

    int w = _img->width();
    int h = _img->height();

    QImage img(w, h, QImage::Format_ARGB32);

    img.fill(QColor(0, 0, 0));

    int countTotal = 0;
    std::map<int, int>::iterator it = _percentMap.begin();
    while (it != _percentMap.end())
    {
        countTotal += it->second;
        it++;
    }

    it = _percentMap.begin();
    while (it != _percentMap.end())
    {
        int yloc = it->first;
        float per = (float)it->second / (float)countTotal;
        int c = (int)(per * 255.0f);
        it++;

        for (int x = 0; x < img.width(); x++)
        {
            img.setPixel(x, yloc, QColor(c, c, c).rgb());
        }
    }


    return img.save(path);
}

//============================================================================
//============================================================================
void ColorPickerGradient::logPercent(double per)
{
    int value = (int)(100 * per);

    std::map<int,int>::iterator it = _percentMap.find(value);
    if (it == _percentMap.end())
    {
        _percentMap[value] = 1;
    }
    else
    {
        _percentMap[value] = _percentMap[value] + 1;
    }
}
