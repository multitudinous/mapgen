#include "utlqt.h"
#include <QDir>
#include <QDateTime>

//============================================================================
//============================================================================
bool UtlQt::fileExists(const char *path)
{
    QFile file(path);
    if (file.exists()) return true;

    return false;
}

//============================================================================
//============================================================================
bool UtlQt::validateDir(const char *dirpath, bool autoCreate)
{
    QDir dir(dirpath);
    if (dir.exists()) return true;

    if (!autoCreate) return false;

    return dir.mkpath(dirpath);
}

//=======================================================================
//=======================================================================
QString UtlQt::pathCombine(const QString &path1, const QString &path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

//=======================================================================
//=======================================================================
QString UtlQt::getDateTimeYMDHMS()
{
    QDateTime dt = QDateTime::currentDateTime();
    QDate date = dt.date();
    QTime time = dt.time();

    char ac[512];
    sprintf(ac, "%d%02d%02d_%02d%02d%02d", date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second());
    return QString(ac);
}

//============================================================================
//============================================================================
void UtlQt::findFiles(const QString &dirPath, const QString &search, std::vector<QString> *plist, bool checkSubDirs)
{
    IteratorBackInsertQStringVector it(*plist);

    UtlQt::findFilesT<IteratorBackInsertQStringVector>(dirPath, search, it, checkSubDirs);
}

//============================================================================
//============================================================================
void UtlQt::findFiles(const QString &dirPath, const QString &search, std::list<QString> *plist, bool checkSubDirs)
{
    IteratorBackInsertQStringList it(*plist);

    UtlQt::findFilesT<IteratorBackInsertQStringList>(dirPath, search, it, checkSubDirs);
}

/*
//============================================================================
//============================================================================
void UtlQt::findFiles(const QString &dirPath, const QString &search, std::vector<QString> *plist, bool checkSubDirs)
{
    QDir dir(dirPath);
    QStringList files = dir.entryList(QStringList(search));

    if (files.size() > 0)
    {
        // add all files found
        for (int i=0; i<mtFiles.size(); i++)
        {
            //item->folderName = dir.dirName();
            //item->fileName = mtFiles[i];
            plist->push_back(UtlQt::pathCombine(dir.absolutePath(), files[i]));
        }
    }

    if (!checkSubDirs) return;

    QString curDir = QDir::cleanPath(dirPath);

    // find any sub directories and recurse
    QFileInfoList subDirs = dir.entryInfoList(QDir::AllDirs);
    for (int i=0; i<subDirs.size(); i++)
    {
        if (!subDirs[i].isDir()) continue;

        QString name = subDirs[i].fileName();
        if (name == "." || name == "..")
        {
            continue;
        }

        QString subDir = subDirs[i].absoluteFilePath();
        if (QDir::cleanPath(subDir) == curDir)
        {
            continue;
        }

        findFiles(subDir, search, plist, true);
    }
}
*/
