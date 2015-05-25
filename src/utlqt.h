#ifndef UTLQT_H
#define UTLQT_H

#include <vector>
#include <list>
#include <QString>
#include <QDir>
#include <QFileInfoList>

class UtlQt
{
public:

    static QString newline() { return QString("\r\n"); }

    static bool fileExists(const char *path);
    static bool validateDir(const char *dir, bool autoCreate=true);
    static QString pathCombine(const QString &path1, const QString &path2);

    static QString getDateTimeYMDHMS();

    static void findFiles(const QString &dirPath, const QString &search, std::vector<QString> *plist, bool checkSubDirs=false); // search = QString("*.ext")
    static void findFiles(const QString &dirPath, const QString &search, std::list<QString> *plist, bool checkSubDirs=false); // search = QString("*.ext")

    static bool findFile(const char *file, const std::vector<std::string> &searchPaths, std::string *pathFound);
    static bool findFile(const char *file, const std::vector<std::string> &searchPaths, const std::vector<std::string> &searchExts, std::string *pathFound);
    static bool findFileFromExt(const char *file, const std::vector<std::string> &searchExt, std::string *pathFound);


protected:
    typedef std::back_insert_iterator< std::vector<QString> > IteratorBackInsertQStringVector;
    typedef std::back_insert_iterator< std::list<QString> > IteratorBackInsertQStringList;

    //============================================================================
    //============================================================================
    template<class BackInsertIterator>
    static void findFilesT(const QString &dirPath, const QString &search, BackInsertIterator &it, bool checkSubDirs)
    {
        QDir dir(dirPath);
        QStringList files = dir.entryList(QStringList(search));

        if (files.size() > 0)
        {
            // add all files found
            for (int i=0; i<files.size(); i++)
            {
                //item->folderName = dir.dirName();
                //item->fileName = mtFiles[i];
                *it = UtlQt::pathCombine(dir.absolutePath(), files[i]);
                it++;
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

            findFilesT(subDir, search, it, true);
        }
    }
};

#endif
