//#include "stdafx.h"
#include "utlstring.h"
#include <stdarg.h>
#include <algorithm>
#include <cstring>

//============================================================================
//============================================================================
std::string UtlString::GetPath(const char *filePath, bool includeLastSep)
{
	std::string path;
	bool foundSep = false;
	int lenToSep = 0;
    int lenTot = (int)strlen(filePath);

	if (lenTot <= 0) return path;

    for (int i=lenTot-1; i>0; i--)
	{
		lenToSep++;
		if (filePath[i] == '\\' || filePath[i] == '/')
		{
			foundSep = true;
			break;
		}
	}

	if (!foundSep) return path;

	path = filePath;
	if (includeLastSep) lenToSep--;
	if (lenToSep > 0) path.erase(lenTot - lenToSep, lenToSep);

	return path;
}

//============================================================================
//============================================================================
std::string UtlString::GetFilename(const char *filePath)
{
	std::string path;
	bool foundSep = false;
	int lenToSep = 0;
    int lenTot = (int)strlen(filePath);

	if (lenTot <= 0) return path;

	for (int i=lenTot-1; i>0; i--)
	{
		lenToSep++;
		if (filePath[i] == '\\' || filePath[i] == '/')
		{
			foundSep = true;
			break;
		}
	}

	if (!foundSep)
	{
		path = filePath;
		return path;
	}

	path = filePath;
	int numToErase = lenTot - lenToSep + 1;
	if (numToErase <= 0) return path;
	path.erase(0, numToErase);

	return path;
}

//============================================================================
//============================================================================
std::string UtlString::RemoveExtension(const char *file, std::string *pExt)
{
	std::string fileName;
	bool foundSep = false;
	int lenToSep = 0;
    int lenTot = (int)strlen(file);

	if (pExt) *pExt = "";
	if (lenTot <= 0) return fileName;

	for (int i=lenTot-1; i>0; i--)
	{
		lenToSep++;
		if (file[i] == '.')
		{
			foundSep = true;
			break;
		}
	}

	if (!foundSep)
	{
		fileName = file;
		return fileName;
	}

	fileName = file;
	fileName.erase(lenTot - lenToSep, lenToSep);

	if (pExt)
	{
		*pExt = file;
		pExt->erase(0, lenTot - lenToSep);
	}

	return fileName;
}

//============================================================================
//============================================================================
std::string UtlString::GetExtension(const char *filePath)
{
    std::string ext = filePath;
    size_t index = ext.rfind(".");
    ext = ext.substr(index+1);

    return ext;
}

//============================================================================
//============================================================================
bool UtlString::GetList(const char *src, std::vector<int> *pResult)
{
	if (!src || !pResult) return false;

    int len = (int)strlen(src);
	if (len <= 0) return false;

	std::string scur;
	int loc = 0;
	int fnd = 0;
	while (1)
	{
		if (loc >= len)
		{
			if (scur.size())
			{
				pResult->push_back( std::stoi(scur) );
				scur.clear();
				fnd++;
			}
			break;
		}

		char c = src[loc];
		if (c == ',')
		{
			if (scur.size())
			{
				pResult->push_back( std::stoi(scur) );
				scur.clear();
				fnd++;
			}
		}
		else
		{
			scur += c;
		}

		loc++;
	}

	if (fnd) return true;
	return false;
}

//============================================================================
//============================================================================
void UtlString::AddDirSlash(std::string &path)
{
  if (path.size() <= 0) return;

    // remove any slash at the end if the item being changed is a directory
  if (path[path.size()-1] != '\\' && path[path.size()-1] != '/') path.push_back('\\');
}

//============================================================================
//============================================================================
void UtlString::format(std::string& a_string, const char* fmt, ...)
{
    /*va_list vl;
    va_start(vl, fmt);
    int size = _vscprintf( fmt, vl );
    a_string.resize( ++size );
    vsnprintf_s((char*)a_string.data(), size, _TRUNCATE, fmt, vl);
    va_end(vl);*/
}

std::string UtlString::format(const char* fmt, ...)
{
    std::string a_string;
    /*va_list vl;
    va_start(vl, fmt);
    int size = _vscprintf( fmt, vl );
    a_string.resize( ++size );
    vsnprintf_s((char*)a_string.data(), size, _TRUNCATE, fmt, vl);
    va_end(vl);*/

    return a_string;
}

//============================================================================
//============================================================================
int UtlString::explode(std::string str, const std::string &separator, std::vector<std::string>* results)
{
    int found, count=0;
    found = str.find_first_of(separator);
    while(found != std::string::npos)
    {
        if (found == 0 && count > 0)
        {
            results->push_back(std::string("")); // empty string
        }
        else if(found > 0)
        {
            results->push_back(str.substr(0,found));
            count++;
        }
        str = str.substr(found+1);
        found = str.find_first_of(separator);
    }

    if(str.length() > 0)
    {
        results->push_back(str);
        count++;
    }

    return count;
}

//============================================================================
//============================================================================
bool UtlString::isNumeric(char c)
{
    if (c >= '0' && c <= '9')
    {
       return true;
    }

    return false;
}

//============================================================================
//============================================================================
bool UtlString::isNumeric(const std::string &s)
{
    if (s.size() <=0 ) return false;
    for (unsigned int i=0; i<s.size(); i++)
    {
        if (!isNumeric(s[i]))
        {
            return false;
        }
    }

    return true;
}

//============================================================================
//============================================================================
bool UtlString::hasNumeric(const std::string &s)
{
    for (unsigned int i=0; i<s.size(); i++)
    {
        if (isNumeric(s[i]))
        {
            return true;
        }
    }

    return false;
}

//============================================================================
//============================================================================
void UtlString::toLower(const std::string &s, std::string *lower)
{
    *lower = s;
    std::transform(lower->begin(), lower->end(), lower->begin(), ::tolower);
}

//============================================================================
//============================================================================
std::string UtlString::toLower(const std::string &s)
{
    std::string lower;
    toLower(s, &lower);
    return lower;
}


//============================================================================
//============================================================================
void UtlString::replaceString(std::string& subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

//============================================================================
//============================================================================
std::string UtlString::replaceStringCopy(std::string subject, const std::string& search, const std::string& replace)
{
    replaceString(subject, search, replace);
    return subject;

    /*
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
    */
}
