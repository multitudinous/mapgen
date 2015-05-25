#include "platform.h"

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

//============================================================================
//============================================================================
void Platform::getFontSearchPaths(std::vector<std::string> *list)
{
#ifdef WIN32
    list->push_back(std::getenv("WINDIR") + std::string("/fonts/"));
#endif

#ifdef _APPLE_
    list->push_back(std::string("/System/Library/Fonts/"));
    list->push_back(std::string("/Library/Fonts/"));
    list->push_back(std::string("~/Library/Fonts/"));
    list->push_back(std::string("/Network/Library/Fonts/"));
#endif

#if defined(LINUX) || defined(__linux)
    list->push_back(std::string("/usr/share/fonts/"));
    list->push_back(std::string("~/.fonts/"));
#endif
}

/*

This is going to be one of those 'simple' problems could have an over-the-top solution depending on what you need this information for.

I will have to apologize for the vaguer Linux answers, as font management across Linux distributions are not consistent and can be very configurable, can be influenced by desktop environment, can be remotely served, etc.

Checking for environment

You can check various platforms via the use of macros defined for specific environments.

Windows - #if defined(_WIN32)
_WIN32 is defined for both 32-bit and 64-bit Windows.
Mac OSX - #if defined(_APPLE_) && defined(_MACH_)
_APPLE_ is defined for all Apple computers, and _MACH_ is defined if the system supports Mach system calls, a la Mac OSX
Linux (generic) - #if defined(linux) || defined(__linux)


Font directory locations
-----------------------------------
Windows
On Windows newer than 3.1, the font directory is located in %WINDIR%\fonts.

Mac OS X
Mac OSX has multiple font directories
/System/Library/Fonts - Fonts necessary for the system. Do not touch these.
/Library/Fonts - Additional fonts that can be used by all users. This is generally where fonts go if they are to be used by other applications.
~/Library/Fonts - Fonts specific to each user.
/Network/Library/Fonts - Fonts shared for users on a network.

Linux
As mentioned above, a Linux distribution may not have specified font directories at all. I remember dealing with this issue a while back since Linux distros don't use any specific font management.
There could be an XFS (X Font Server) serving up fonts remotely.
The most common locations for fonts across Linux distributions are /usr/share/fonts, /usr/local/share/fonts, and user-specific ~/.fonts
Some systems may have configured font directories in the file /etc/fonts/fonts.conf or /etc/fonts/local.conf.

*/