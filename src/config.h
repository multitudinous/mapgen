#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <memory>
#include "rgbf.h"
#include "extents.h"

class Config
{
public:
    Config();

    bool windowless();

    const std::string& mode() const { return _mode; }
    void mode(const std::string &mode) { _mode = mode; }

    bool toDisk() const { return _toDisk; }
    void toDisk(bool set) { _toDisk = set; }

    int width() const { return _width; }
    void width(int w) { _width = w; }

    int height() const { return _height; }
    void height(int h) { _height = h; }

    const std::string& imgFile() const { return _imgfile; }
    void imgFile(const std::string &file);
    const std::string& imgFolder() const { return _imgfolder; }

    const std::string& outType() const { return _outtype; }
    void outType(const std::string &t) { _outtype = t; }

    const std::string& dataFile() const { return _datafile; }
    void dataFile(const std::string &file) { _datafile = file; }

    bool lyrOutMode() const { return _lyrOutMode;  }
    void lyrOutMode(bool on) { _lyrOutMode = on;  }

    const Rgbf& colrClear() const { return _colrClear;  }
    void colrClear(const Rgbf &rgb) { _colrClear = rgb;  }

    const Extents& mapExtents() const { return _mapExtents;  }
    void mapExtents(const Extents &e) { _mapExtents = e; }
    bool mapExentsValid() const { return (fabs(_mapExtents.area()) > 0.0001); }

    bool msaaOn() const { return _msaaOn;  }
    void  msaaOn(bool on) { _msaaOn = on; }
    int msaaSamples() const { return _samples;  }
    void msaaSamples(int samples) { _samples = samples; }

protected:
    std::string _mode; // windowless, desktop

    // output
    bool _toDisk;
    int _width;
    int _height;
    std::string _imgfolder;
    std::string _imgfile;
    bool _lyrOutMode; // render each layer to its own image with a transparent background
    Rgbf _colrClear;

    std::string _outtype;
    std::string _datafile;

    Extents _mapExtents;

    bool _msaaOn;
    int _samples;
};

typedef std::shared_ptr<Config> PConfig;

#endif
