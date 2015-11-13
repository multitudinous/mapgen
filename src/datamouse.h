#ifndef DATAMOUSE
#define DATAMOUSE

#include "sysdefs.h"

class DataMouse
{
public:
    enum EMouseBtn
    {
        MOUSE_LEFT_BUTTON = 0,
        MOUSE_MIDDLE_BUTTON = 1,
        MOUSE_RIGHT_BUTTON = 2,
        MOUSE_SCROLL_UP = 3,
        MOUSE_SCROLL_DOWN = 4
    };

    enum EMouseBtnDown
    {
        MOUSE_DOWN_NONE = 0,
        MOUSE_DOWN_LBTN = 1,
        MOUSE_DOWN_MBTN = 2,
        MOUSE_DOWN_RBTN = 4
    };

public:
    DataMouse();

    int& x() { return _x;  }
    int& y() { return _y; }
    int& prevx() { return _prevx; }
    int& prevy() { return _prevy; }
    int& btnsDown() { return _btnsDown; }
    bool& firstDown() { return _firstDown; }

protected:
    int _x;
    int _y;
    int _prevx;
    int _prevy;
    int _btnsDown;
    bool _firstDown;
};

typedef shared_ptr<DataMouse> PDataMouse;

#endif