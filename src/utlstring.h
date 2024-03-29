#ifndef UTLSTRING_H
#define UTLSTRING_H

#include <string>
#include <vector>

class UtlString
{
public:
	static std::string getPath(const char *filePath, bool includeLastSep=true);
	static std::string getFilename(const char *filePath);
	static std::string removeExtension(const char *file, std::string *pExt=NULL);
    static std::string getExtension(const char *filePath);
	static void addDirSlash(std::string &path);

	static bool getList(const char *src, std::vector<int> *pResult);

    static int explode(std::string str, const std::string &separator, std::vector<std::string>* results);

    static void format(std::string& a_string, const char* fmt, ...);
    static std::string format(const char* fmt, ...);

    static bool isNumeric(char c);
    static bool isNumeric(const std::string &s);
    static bool hasNumeric(const std::string &s);

    static void toLower(const std::string &s, std::string *lower);
    static std::string toLower(const std::string &s);

    static std::string triml(const std::string &s);
    static std::string trimr(const std::string &s, char rc = ' ');
    static std::string trim(const std::string &s);

    static void replaceString(std::string& subject, const std::string& search, const std::string& replace);
    static std::string replaceStringCopy(std::string subject, const std::string& search, const std::string& replace);

};

#endif
