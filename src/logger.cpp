#include "logger.h"
#include <stdarg.h>
#include <string>
#include <iostream>

bool gLogXml = false;

void LogError(const tchar *format, ...)
{
#if WIN32
	tchar buffer[10240];
	va_list args;

	va_start(args, format);
	vsprintf_s(buffer, 10240, format, args);
	va_end(args);

	std::string strOut;
	if (gLogXml)
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

	LogMsg(strOut.c_str());
#endif
}

void LogWarning(const tchar *format, ...)
{
#if WIN32
	tchar buffer[10240];
	va_list args;

	va_start(args, format);
	vsprintf_s(buffer, 10240, format, args);
	va_end(args);

	std::string strOut;
	if (gLogXml)
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

	LogMsg(strOut.c_str());
#endif
}

void LogTrace(const tchar *format, ...)
{
#if WIN32
	tchar buffer[10240];
	va_list args;

	va_start(args, format);
	vsprintf_s(buffer, 10240, format, args);
	va_end(args);

	std::string strOut;
	if (gLogXml)
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

	LogMsg(strOut.c_str());
#endif
}

void LogMsg(const tchar *pacMsg)
{
#ifdef _MSC_VER
	//OutputDebugString(pacMsg);
#endif
	std::cout << pacMsg << std::endl;
}
