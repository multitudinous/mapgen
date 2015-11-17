#ifndef UTLQT_H
#define UTLQT_H

#include <vector>
#include <list>
#include <memory>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QPainter>
#include <QImage>
#include <QFont>

class MemBuf;

typedef std::shared_ptr< QPainter > PQPainter;
typedef std::shared_ptr< QPaintDevice > PQPaintDevice;
typedef std::shared_ptr< QImage > PQImage;
typedef std::shared_ptr< QLinearGradient > PQLinearGradient;
typedef std::shared_ptr< QFont > PQFont;


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

    static QRectF rectWithMargins(double left, double top, double width, double height, double ml, double mt, double mr, double mb);
    static QRect rectWithMargins(int left, int top, int width, int height, int ml, int mt, int mr, int mb);

    static int textHeight(const QFont &font);
    static QRect textSize(const QFont &font, const QString &text);
    static QRect textRectVertTop(int left, int top, int txWidth, int txHeight);
    static QRect textRectVertCtr(int left, int center, int txWidth, int txHeight);
    static QRect textRectVertBtm(int left, int btm, int txWidth, int txHeight);

    static PQLinearGradient gradientRamp(const QRect &rcBar, const QColor &minc, const QColor &midc, const QColor &maxc);
    static PQImage gradientPicker(const QColor &minc, const QColor &midc, const QColor &maxc);
    static QColor gradientPick(const QImage &picker, float percent);

    static bool saveImg(MemBuf *imgbuf, const char *file);

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

