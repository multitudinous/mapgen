#ifndef BINGIMG_H
#define BINGIMG_H

#include "extents.h"

#include <QObject>
#include <QTNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QTNetwork/QNetworkRequest>
#include <QTNetwork/QNetworkReply>
#include <QTGui/QImage>

typedef std::tr1::shared_ptr<QImage> PQImage;

class BingImg : public QObject
{
    Q_OBJECT
public:
    explicit BingImg(const char *cacheFolder="c:/mapgen/cache/bing/", bool useCache=true, QObject *parent = 0);
    
    bool query(const Extents &ext);
    bool done() const;

    PQImage getImg() { return _img; }
    Extents getExtents() { return _ext; }

    bool setCachePath(const char *path);

signals:
    
public slots:
    void downloadFinished();

protected:
    void onDone(PQImage img);

    void initExtents(const Extents &ext);

    bool haveCache();
    bool saveToCache(QImage *img);
    std::string getCachePath(const Extents &ext) const;
    bool validateCache();

    std::string createUrlDebug(const Extents &ext);
    std::string createUrlWms();


protected:
    QNetworkAccessManager *_nam; // TODO: probably just one of these and pass it in instead of one for each image
    QNetworkReply *_reply;

    Extents _ext;
    PQImage _img;
    bool _querying;

    bool _useCache;
    std::string _cacheFolder;
    
};

#endif // BINGIMG_H
