#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <stdarg.h>

//============================================================================
//============================================================================
#if WIN32
    #define vsprintf vsprintf_s
#endif

//============================================================================
//============================================================================
#ifndef WIN32
int _vscprintf(const char * format, va_list pargs)
{
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}
#endif

#endif