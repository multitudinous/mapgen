#include "colorpickercat.h"
#include "utlstring.h"
#include <fstream>

//============================================================================
//============================================================================
ColorPickerCat::ColorPickerCat()
{

}

//============================================================================
//============================================================================
void ColorPickerCat::begin(const char *id)
{
    if (!id || strlen(id) <= 0) return;

    std::map<std::string, PColorMap>::iterator it = _colorsused.find(id);
    if (it != _colorsused.end())
    {
        _curColorsUsed = it->second;
        return;
    }

    _curColorsUsed.reset(new ColorMap());
    _colorsused[id] = _curColorsUsed;


}

//============================================================================
//============================================================================
void ColorPickerCat::end()
{
    if (!_curColorsUsed) return;

    LogTrace("ColorPickerCat::end() - categories used...");
    ColorMap::const_iterator it = _curColorsUsed->begin();
    while (it != _curColorsUsed->end())
    {
        int r = it->second->c.red();
        int g = it->second->c.green();
        int b = it->second->c.blue();
        int a = it->second->c.alpha();
        LogTrace("value: %d hex: %s, rgb(%d,%d,%d,%d), name: %s", it->second->value, it->second->hex.c_str(), r, g, b, a, it->second->name.c_str());

        it++;
    }
}

//============================================================================
//============================================================================
ColorPickerCat::PColorMap ColorPickerCat::getColorUsed(const char *id)
{
    if (!id || strlen(id) <= 0) return PColorMap();

    std::map<std::string, PColorMap>::iterator it = _colorsused.find(id);
    if (it == _colorsused.end())
    {
        return PColorMap();
    }

    return it->second;
}

//============================================================================
//============================================================================
ColorPickerCat::PColorVec ColorPickerCat::getColorUsedVec(const char *id)
{
    PColorMap map = getColorUsed(id);
    if (!map)
    {
        return PColorVec();
    }

    PColorVec vec(new ColorVec());
    ColorMap::const_iterator it = map->begin();
    while (it != map->end())
    {
        vec->push_back(it->second);
        it++;
    }

    return vec;

}

//============================================================================
//============================================================================
QColor ColorPickerCat::pickPrefered(float percent, double value, bool validation)
{
    // currently don't have validation for this
    return pickPrefered(percent, value);
}

//============================================================================
//============================================================================
QColor ColorPickerCat::pickPrefered(float percent, double value) const
{
    return pickByValue(value);
}

//============================================================================
//============================================================================
QColor ColorPickerCat::pickByPercent(float per) const
{
    if (_valvec.size() <= 0)
    {
        return QColor(0, 0, 0, 255);
    }

    int vloc = (_valvec.size() * per) -1;
    if (vloc >= _valvec.size()) vloc--;
    if (vloc < 0) vloc = 0;

    return pickByValue(_valvec[vloc]);
    
}

//============================================================================
//============================================================================
QColor ColorPickerCat::pickByValue(double val) const
{
    return pickByValue((int)val);
}

//============================================================================
//============================================================================
QColor ColorPickerCat::pickByValue(int val) const
{
    ColorMap::const_iterator it = _colormap.find(val);
    if (it == _colormap.end()) return QColor(0, 0, 0, 255);

    // add this color as being used
    if (_curColorsUsed)
    {
        (*_curColorsUsed)[it->second->value] = it->second;
    }

    return it->second->c;
}

//============================================================================
//============================================================================
QColor ColorPickerCat::getMin() const
{
    return _colormin;
}

//============================================================================
//============================================================================
QColor ColorPickerCat::getMid() const
{
    return _colormid;
}

//============================================================================
//============================================================================
QColor ColorPickerCat::getMax() const
{
    return _colormax;
}

//============================================================================
//============================================================================
bool ColorPickerCat::initFromFile(const char *filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return false;
    }

    _colormap.clear();
    _valvec.clear();
    _colormin = QColor(0, 0, 0, 255);
    _colormid = QColor(0, 0, 0, 255);
    _colormax = QColor(0, 0, 0, 255);

    int linenum = 0;
    bool haveColNames = false;
    std::string line;
    while (std::getline(file, line))
    {
        linenum++;

        if (line.size() <= 1)
        {
            // skip empty lines
            continue;
        }

        if (!haveColNames)
        {
            if (line.find_first_of("value") != string::npos)
            {
                haveColNames = true;
            }

            continue;
        }

        
        PCat cat(new SCat());
        int locNameStart = 0;
        int locNameEnd = 0;
        int locHexEnd = 0;

        // find the value
        if (!findValue(line, &cat->value, &locNameStart))
        {
            // failed to find the value, should be first column
            LogError("ColorPickerCat::initFromFile - failed to find the value on line: %d, in file: %s", linenum, filePath);
            continue;
        }

        // find the rgb color
        if (!findRgb(line, &cat->c, &locHexEnd))
        {
            // failed to find the rgb color, should be forth (last) column
            LogError("ColorPickerCat::initFromFile - failed to find the rgb color on line: %d, in file: %s", linenum, filePath);
            continue;
        }

        // find the hex
        if (!findHex(line, locHexEnd, &cat->hex, &locNameEnd))
        {
            // failed to find the hex color, should be third column
            LogError("ColorPickerCat::initFromFile - failed to find the hex color on line: %d, in file: %s", linenum, filePath);
            continue;
        }


        if (!findName(line, locNameStart, locNameEnd, &cat->name))
        {
            // failed to find the name, should be second column
            LogError("ColorPickerCat::initFromFile - failed to find the category on line: %d, in file: %s", linenum, filePath);
            continue;
        }

        if (_colormap.find(cat->value) != _colormap.end())
        {
            LogError("ColorPickerCat::initFromFile - duplicate value: %d on line: %d, in file: %s", cat->value, linenum, filePath);
            continue;
        }

        _colormap[cat->value] = cat;
        _valvec.push_back(cat->value);
    }

    file.close();

    if (_valvec.size() > 0)
    {
        std::sort(_valvec.begin(), _valvec.end(), std::less<int>());

        int idxmin = 0;
        int idxmax = (int)_valvec.size() - 1;
        int idxmid = ((int)_valvec.size() / 2) - 1;
        if (idxmid < 0) idxmid = 0;
        if (idxmid >= (int)_valvec.size()) idxmid = (int)_valvec.size() - 1;

        _colormin = _colormap[_valvec[idxmin]]->c;
        _colormid = _colormap[_valvec[idxmid]]->c;
        _colormax = _colormap[_valvec[idxmax]]->c;
    }

    return true;
}

//============================================================================
//============================================================================
bool ColorPickerCat::findValue(const std::string &line, int *val, int *locValNext)
{ 
    // find the value
    string sval = "";
    size_t loc = 0;
    while (loc < line.size())
    {
        if (line[loc] == ' ' || line[loc] == '\t')
        {
            if (sval.size() <= 0)
            {
                loc++;
                continue; // strip any leading white space or tabs
            }
            else
            {
                *locValNext = (int)loc;
                break;
            }
        }

        if (line[loc] >= '0' && line[loc] <= '9')
        {
            sval += line[loc];
            loc++;
            continue;
        }

        *locValNext = (int)loc;
        break; // first non numeric character.
    }

    // did we find the value
    if (sval.size() <= 0)
    {
        // failed to find the value, should be first column
        return false;
    }

    *val = atoi(sval.c_str());
    return true;
}

//============================================================================
//============================================================================
bool ColorPickerCat::findRgb(const std::string &line, QColor *c, int *locValPrev)
{
    // find the rgb color
    string scolr = "";
    int loc = (int)line.size() - 1;
    bool foundEnd = false;
    bool foundStart = false;
    while (loc >= 0)
    {
        if (line[loc] == ')')
        {
            if (foundEnd)
            {
                // error, found 2 ends before the start of the (r,g,b,a)
                scolr.clear();
                break;
            }

            foundEnd = true;
            loc--;
            continue;
        }

        // strip away anything before we find the end of the rgb string (going backwards, so end is the starting point for us)
        if (!foundEnd)
        {
            loc--;
            continue;
        }

        if (line[loc] == '(')
        {
            foundStart = true;
            *locValPrev = loc - 1;
            break; // done
        }

        scolr = line[loc] + scolr;
        loc--;
    }

    if (!foundEnd || !foundStart)
    {
        return false;
    }

    std::vector<std::string> rgba;
    UtlString::explode(scolr, std::string(","), &rgba);
    if (rgba.size() < 3)
    {
        return false;
    }

    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
    r = atoi(rgba[0].c_str());
    g = atoi(rgba[1].c_str());
    b = atoi(rgba[2].c_str());
    if (rgba.size() > 3)
    {
        a = atoi(rgba[3].c_str());
    }

    *c = QColor(r, g, b, a);
    return true;
}

//============================================================================
//============================================================================
bool ColorPickerCat::findHex(const std::string &line, int locEnd, std::string *hex, int *locValPrev)
{
    // find the hex color
    *hex = "";

    int loc = locEnd;
    bool foundEnd = false;
    bool foundStart = false;
    while (loc >= 0)
    {
        if (line[loc] == '#')
        {
            foundStart = true;
            *locValPrev = loc - 1;
            break; // done
        }

        if (isalnum(line[loc]))
        {
            foundEnd = true;
            *hex = line[loc] + *hex;
            loc--;
            continue;
        }

        if (foundEnd)
        {
            // invalid character if we already found the start of the hex (an alphanumeric), but haven't reach the start (#) yet
            return false;
        }

        loc--;
        continue;     
    }

    if (!foundEnd || !foundStart)
    {
        return false;
    }

    if (hex->size() < 6)
    {
        return false; // should be 8 but 6 without alpha
    }


    return true;
}

//============================================================================
//============================================================================
bool ColorPickerCat::findName(const std::string &line, int locStart, int locEnd, std::string *name)
{
    *name = "";

    if (locEnd < locStart)
    {
        return false;
    }

    size_t len = (locEnd - locStart) + 1;
    if (len <= 0)
    {
        return true; // empty name but not an error
    }

    // copy what we need from the line
    std::string n = line.substr(locStart, len);

    // strip white space and tabs
    size_t i = 0;
    while (i < n.size())
    {
        locStart = (int)i;
        if (n[i] == ' ' || n[i] == '\t')
        {
            i++;
            continue;
        }

        break; // done
    }

    i = n.size() - 1;
    while (i >= 0)
    {
        locEnd = (int)i;
        if (n[i] == ' ' || n[i] == '\t')
        {
            if (i == 0) break; // all done

            i--;
            continue;
        }

        break; // done
    }

    if (locEnd <= locStart)
    {
        return true; // empty name
    }

    len = (locEnd - locStart) + 1;
    *name = n.substr(locStart, len);
    return true;
}