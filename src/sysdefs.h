#ifndef SYSDEFS_H
#define SYSDEFS_H

#include "logger.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <memory>
using namespace std;

#include <GL/glew.h>
#ifdef _WIN32
#define NOMINMAX
#include <wglew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GL/glut.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#define QT

#ifndef DWORD
    #define DWORD unsigned long
#endif

#ifndef _WIN32
    typedef int8_t  BYTE;
#endif

#endif
