#include "filepaths.h"
#include "utlstring.h"

#ifdef QT
    #include "utlqt.h"
#endif

FilePaths::FilePaths()
{
}

void FilePaths::Init(const std::string &exePath)
{
	m_pathExe = exePath;
	m_appName = UtlString::getFilename(m_pathExe.c_str()).c_str();
	m_pathBin = UtlString::getPath(m_pathExe.c_str()).c_str();
    m_pathCache  = m_pathBin + "res/cache/";
    m_pathImgOut = m_pathBin + "res/img/output/";

    m_pathValidate = m_pathBin + "res/validate/";
    m_pathData = m_pathBin + "res/data/";
    m_pathVidProf = m_pathBin + "res/video/profiles/";
    m_pathVidOut = m_pathBin + "res/video/output/";
    m_pathXml = m_pathBin + "res/xml/";

#ifdef QT
    UtlQt::validateDir(m_pathCache.c_str(), true);
    UtlQt::validateDir(m_pathImgOut.c_str(), true);
#endif
}
