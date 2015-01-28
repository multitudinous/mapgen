#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <memory>

class Config
{
public:
    Config();

    bool windowless();

    const std::string& mode() { return _mode; }
    void mode(const std::string &mode) { _mode = mode; }

    bool toDisk() { return _toDisk; }
    void toDisk(bool set) { _toDisk = set; }

    int width() { return _width; }
    void width(int w) { _width = w; }

    int height() { return _height; }
    void height(int h) { _height = h; }

    const std::string& imgFile() { return _imgfile; }
    void imgFile(const std::string &file) { _imgfile = file; }

    const std::string& outType() { return _outtype; }
    void outType(const std::string &t) { _outtype = t; }

    const std::string& dataFile() { return _datafile; }
    void dataFile(const std::string &file) { _datafile = file; }

protected:
    std::string _mode; // windowless, desktop

    // output
    bool _toDisk;
    int _width;
    int _height;
    std::string _imgfile;

    std::string _outtype;
    std::string _datafile;
};

typedef std::shared_ptr<Config> PConfig;

#endif
