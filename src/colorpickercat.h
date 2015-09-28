#ifndef COLORPICKERCAT_H
#define COLORPICKERCAT_H

#include "colorpicker.h"

class ColorPickerCat : public ColorPicker
{
public:
    struct SCat
    {
        int value;
        std::string name;
        std::string hex;
        QColor c;

        SCat()
        {
            value = 0;
            c = QColor(0, 0, 0, 255);
        }
    };
    typedef std::shared_ptr<SCat> PCat;
    typedef std::map<int, PCat> ColorMap;
    typedef std::shared_ptr<ColorMap> PColorMap;
    typedef std::vector<PCat> ColorVec;
    typedef std::shared_ptr<ColorVec> PColorVec;

public:
    ColorPickerCat();

    virtual QColor pickPrefered(float percent, double value) const;
    virtual QColor pickByPercent(float percent) const;
    virtual QColor pickByValue(double value) const;
    virtual QColor pickByValue(int value) const;

    virtual QColor getMin() const;
    virtual QColor getMid() const;
    virtual QColor getMax() const;

    virtual bool hasPickPercent() const { return true; }
    virtual bool hasPickValue() const { return true; }

    virtual void begin(const char *id); // store categories that were selected
    virtual void end();

    PColorMap getColorUsed(const char *id);
    PColorVec getColorUsedVec(const char *id);

    bool initFromFile(const char *file);

protected:

    bool findValue(const std::string &line, int *val, int *locValNext);
    bool findRgb(const std::string &line, QColor *c, int *locValPrev);
    bool findHex(const std::string &line, int locEnd, std::string *hex, int *locValPrev);
    bool findName(const std::string &line, int locStart, int locEnd, std::string *name);

protected:
    ColorMap _colormap;
    std::vector<int> _valvec;
    QColor _colormin;
    QColor _colormid;
    QColor _colormax;

    std::map<std::string, PColorMap> _colorsused;
    PColorMap _curColorsUsed;
};

#endif