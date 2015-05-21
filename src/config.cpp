#include "config.h"
#include "utlstring.h"

Config::Config()
{
    _mode = "desktop";
    _toDisk = true;
    _width = 1024;
    _height = 1024;
    _imgfile = "map.png";
    _outtype = "";
    _datafile = "data.txt";
    _lyrOutMode = true;
    _colrClear.SetColor(1, 1, 1, 0); // alpha 0 for output, but will be set to 1 for onscreen rendering
}

bool Config::windowless()
{
    if (_mode == "windowless") return true;
    return false;
}

void Config::imgFile(const std::string &file)
{ 
    _imgfile = file; 

    _imgfolder = UtlString::GetPath(_imgfile.c_str(), true);
}
