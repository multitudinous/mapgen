#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#ifndef tchar
	#define tchar char
#endif


void LogError(const tchar *format, ...);
void LogWarning(const tchar *format, ...);
void LogTrace(const tchar *format, ...);
void LogMsg(const tchar *pacMsg);

class Logger
{
public:
    static void init(bool logToFile, const char *file, bool logXml = false);
    static void shutdown();

    static void logMsg(const tchar *pacMsg);

    static bool logXml();

protected:
    static FILE *_fp;
    static bool _logXml;
};
#endif
