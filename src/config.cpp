#include "config.h"

Config::Config()
{
    _mode = "desktop";
    _toDisk = true;
    _width = 1024;
    _height = 1024;
    _imgfile = "map.png";
    _outtype = "";
    _datafile = "data.txt";
}

bool Config::windowless()
{
    if (_mode == "windowless") return true;
    return false;
}
