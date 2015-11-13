#include "datamouse.h"

DataMouse::DataMouse() :
    _x(0),
    _y(0),
    _prevx(0),
    _prevy(0),
    _btnsDown(MOUSE_DOWN_NONE),
    _firstDown(false)
{

}