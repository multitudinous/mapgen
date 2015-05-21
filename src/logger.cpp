#include "logger.h"
#include <stdarg.h>
#include <string>
#include <iostream>
#include "platform.h"

FILE *Logger::_fp = NULL;
bool Logger::_logXml = false;

//============================================================================
//============================================================================
void Logger::init(bool logToFile, const char *file, bool logXml)
{
    Logger::shutdown();

    _logXml = logXml;

    if (!logToFile) return;
    
    _fp = fopen(file, "wt");
}

//============================================================================
//============================================================================
void Logger::shutdown()
{
    if (_fp)
    {
        fclose(_fp);
        _fp = NULL;
    }
}

//============================================================================
//============================================================================
void Logger::logMsg(const tchar *pacMsg)
{
#ifdef _MSC_VER
	//OutputDebugString(pacMsg);
#endif
	std::cout << pacMsg << std::endl;

    if (_fp)
    {
        fprintf(_fp, "%s", pacMsg);
    }
}

//============================================================================
//============================================================================
bool Logger::logXml()
{
    return _logXml;
}


//============================================================================
//============================================================================
void LogError(const tchar *format, ...)
{
    tchar buffer[10240];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, 10240, format, args);
    va_end(args);

    std::string strOut;
    if (Logger::logXml())
    {
        strOut = "<error>";
        strOut += buffer;
        strOut += "</error>\n";
    }
    else
    {
        strOut = "Error: ";
        strOut += buffer;
        strOut += "\n";
    }

    Logger::logMsg(strOut.c_str());
}

//============================================================================
//============================================================================
void LogWarning(const tchar *format, ...)
{
    //#if WIN32
    tchar buffer[10240];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, 10240, format, args);
    va_end(args);

    std::string strOut;
    if (Logger::logXml())
    {
        strOut = "<warning>";
        strOut += buffer;
        strOut += "</warning>\n";
    }
    else
    {
        strOut = "Warning: ";
        strOut += buffer;
        strOut += "\n";
    }

    Logger::logMsg(strOut.c_str());
}

//============================================================================
//============================================================================
void LogTrace(const tchar *format, ...)
{
    tchar buffer[10240];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, 10240, format, args);
    va_end(args);

    std::string strOut;
    if (Logger::logXml())
    {
        strOut = "<trace>";
        strOut += buffer;
        strOut += "</trace>\n";
    }
    else
    {
        strOut = buffer;
        strOut += "\n";
    }

    Logger::logMsg(strOut.c_str());
}