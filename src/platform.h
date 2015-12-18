#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <string>

//============================================================================
//============================================================================
#if WIN32
    #define vsprintf vsprintf_s
#endif

//============================================================================
//============================================================================
#ifndef WIN32
    int _vscprintf(const char * format, va_list pargs);
#endif


//============================================================================
//============================================================================
#if WIN32
	#define snprintf _snprintf // maybe able to remove this when moving to visual studio 2015
#endif


class Platform
{
public:
    static void getFontSearchPaths(std::vector<std::string> *list);
};

#endif