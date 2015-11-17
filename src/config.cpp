#include "config.h"
#include "utlstring.h"
#include "fborender.h"

Config::Config()
{
    _mode = "window";
    _toDisk = true;
    _width = 1024;
    _height = 1024;
    _imgfile = "map.png";
    _outtype = "";
    _datafile = "data.txt";
    _lyrOutMode = true;
    _colrClear.SetColor(1, 1, 1, 0); // alpha 0 for output, but will be set to 1 for onscreen rendering,

    _msaaOn = false;
    _samples = 16;
    _ssaaOn = false;
    _ssaaMul = 2;
    _jtaaOn = false;
    _jtaaSamp = 16;
    _jtaaoffset = 1;
}

bool Config::windowless()
{
    if (_mode == "windowless") return true;
    return false;
}

void Config::imgFile(const std::string &file)
{ 
    _imgfile = file; 

    _imgfolder = UtlString::getPath(_imgfile.c_str(), true);
}


int Config::getRenderFlags()
{
    int rf = FboRender::E_RF_NONE;
    if (msaaOn()) rf |= FboRender::E_RF_MULTISAMPLE;
    if (ssaaOn()) rf |= FboRender::E_RF_SUPERSAMPLE;
    if (jtaaOn()) rf |= FboRender::E_RF_JITTER;

    return rf;
}