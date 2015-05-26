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

//============================================================================
//============================================================================
bool UtlQt::findFile(const char *file, const std::vector<std::string> &searchPaths, std::string *pathFound)
{
    if (fileExists(file))
    {
        *pathFound = file;
        return true;
    }

    for (unsigned int i = 0; i<searchPaths.size(); i++)
    {
        std::string pathcheck = searchPaths[i] + file;
        if (UtlQt::fileExists(pathcheck.c_str()))
        {
            *pathFound = pathcheck;
            return true;
        }
    }

    return false;
}

//============================================================================
//============================================================================
bool UtlQt::findFile(const char *file, const std::vector<std::string> &searchPaths, const std::vector<std::string> &searchExts, std::string *pathFound)
{
    if (findFileFromExt(file, searchExts, pathFound)) return true;

    for (unsigned int i = 0; i<searchPaths.size(); i++)
    {
        std::string pathcheck = searchPaths[i] + file;
        if (UtlQt::fileExists(pathcheck.c_str()))
        {
            *pathFound = pathcheck;
            return true;
        }

        if (findFileFromExt(pathcheck.c_str(), searchExts, pathFound))
        {
            return true;
        }
    }

    return false;
}

//============================================================================
//============================================================================
bool UtlQt::findFileFromExt(const char *file, const std::vector<std::string> &searchExt, std::string *pathFound)
{
    if (fileExists(file))
    {
        *pathFound = file;
        return true;
    }

    for (unsigned int i = 0; i<searchExt.size(); i++)
    {
        std::string pathcheck = file + searchExt[i];
        if (UtlQt::fileExists(pathcheck.c_str()))
        {
            *pathFound = pathcheck;
            return true;
        }
    }

    return false;
}

//============================================================================
//============================================================================
QRectF UtlQt::rectWithMargins(double left, double top, double width, double height, double ml, double mt, double mr, double mb)
{
    return QRectF(left + ml, top + mt, width - (ml + mr), height - (mt + mb));
}

//============================================================================
//============================================================================
QRect UtlQt::rectWithMargins(int left, int top, int width, int height, int ml, int mt, int mr, int mb)
{
    return QRect(left + ml, top + mt, width - (ml + mr), height - (mt + mb));
}


//============================================================================
//============================================================================
int UtlQt::textHeight(const QFont &font)
{
    QFontMetrics fm(font);
    return fm.height();
}

//============================================================================
//============================================================================
QRect UtlQt::textSize(const QFont &font, const QString &text)
{
    QFontMetrics fm(font);
    return QRect(0, 0, fm.width(text), fm.height());
}

//============================================================================
//============================================================================
QRect UtlQt::textRectVertTop(int left, int top, int txWidth, int txHeight)
{
    return QRect(left, top, txWidth, txHeight);
}

//============================================================================
//============================================================================
QRect UtlQt::textRectVertCtr(int left, int center, int txWidth, int txHeight)
{
    return QRect(left, center - txHeight / 2, txWidth, txHeight);
}

//============================================================================
//============================================================================
QRect UtlQt::textRectVertBtm(int left, int btm, int txWidth, int txHeight)
{
    return QRect(left, btm - txHeight, txWidth, txHeight);
}

//============================================================================
//============================================================================
PQLinearGradient UtlQt::gradientRamp(const QRect &rcBar, const QColor &minc, const QColor &midc, const QColor &maxc)
{
    // gradient color filler
    PQLinearGradient gradient(new QLinearGradient(rcBar.topLeft(), rcBar.bottomRight()));
    gradient->setColorAt(0.0, minc);
    gradient->setColorAt(0.5, midc);
    gradient->setColorAt(1.0, maxc);
    return gradient;
}


//============================================================================
//============================================================================
PQImage UtlQt::gradientPicker(const QColor &minc, const QColor &midc, const QColor &maxc)
{
    QRect rc = QRect(0, 0, 10, 100);
    PQImage picker(new QImage(rc.size(), QImage::Format_ARGB32));

    picker->fill(QColor(0, 0, 0, 255));
    PQLinearGradient grad = gradientRamp(rc, minc, midc, maxc);

    QPainter painter(picker.get());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rc, *grad);
    painter.end();

    //picker->save("picker.png")
    return picker;
}

//============================================================================
//============================================================================
QColor UtlQt::gradientPick(const QImage &picker, float percent)
{
    int x = picker.width() / 2;
    int y = int(float(picker.height()) * percent);

    // clamp
    if (y < 0)
    {
        y = 0;
    }
    if (y >= picker.height())
    {
        y = picker.height() - 1;
    }

    return QColor(picker.pixel(x, y));
}