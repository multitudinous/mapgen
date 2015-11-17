#ifndef FILEPATHS_H
#define FILEPATHS_H

#include <string>
#include <memory>

class FilePaths
{
public:
	FilePaths();

	virtual void Init(const std::string &exePath);

public:
	std::string m_pathBin;
    std::string m_pathCache;
    std::string m_pathImgOut;

    std::string m_pathValidate;
    std::string m_pathData;
	std::string m_pathVidProf;
	std::string m_pathVidOut;
	std::string m_pathXml;
	std::string m_pathExe;
	std::string m_appName;
};

typedef std::shared_ptr<FilePaths> PFilePaths;

#endif
