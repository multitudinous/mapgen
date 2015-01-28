#ifndef UTLSTRING_H
#define UTLSTRING_H

#include <string>
#include <vector>
//#include <tchar.h>

class UtlString
{
public:
	static std::string GetPath(const char *filePath, bool includeLastSep=true);
	static std::string GetFilename(const char *filePath);
	static std::string RemoveExtension(const char *file, std::string *pExt=NULL);
    static std::string GetExtension(const char *filePath);
	static void AddDirSlash(std::string &path);

	static bool GetList(const char *src, std::vector<int> *pResult);

    static int explode(std::string str, const std::string &separator, std::vector<std::string>* results);

    static void format(std::string& a_string, const char* fmt, ...);
    static std::string format(const char* fmt, ...);

    static bool isNumeric(char c);
    static bool isNumeric(const std::string &s);
    static bool hasNumeric(const std::string &s);

    static void toLower(const std::string &s, std::string *lower);
    static std::string toLower(const std::string &s);

    static void replaceString(std::string& subject, const std::string& search, const std::string& replace);
    static std::string replaceStringCopy(std::string subject, const std::string& search, const std::string& replace);

};

#endif
