#ifndef SYSDEFS_H
#define SYSDEFS_H

#include "logger.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
//#include <boost\shared_ptr.hpp>
#include <memory>
using namespace std;
//using namespace std::tr1;

#include <glew.h>
#ifdef _WIN32
#include <wglew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#define QT

#ifndef DWORD
    #define DWORD unsigned long
#endif

#ifndef _WIN32
    typedef int8_t  BYTE;
#endif

#endif
