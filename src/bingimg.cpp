#include "bingimg.h"
#include <QtGui/QImageReader>
#include "utlstring.h"
#include "logger.h"
#include <QFile>
#include <QDir>

//============================================================================
//============================================================================
BingImg::BingImg(const char *cachefolder, bool useCache, QObject *parent) :
    QObject(parent)
{
    _useCache = useCache;
    _querying = false;
    _reply = NULL;

    setCachePath(cachefolder);
}

//============================================================================
//============================================================================
bool BingImg::query(const Extents &ext)
{
    if (_querying) return false;

    initExtents(ext);
    if (haveCache()) return true;

    _querying = true;

    _nam = new QNetworkAccessManager(this);
   // QObject::connect(_nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply*)));

    QUrl url(createUrlWms().c_str());
    if (done()) return true; // loaded from cache

    QNetworkRequest req(url);

    req.setRawHeader("User-Agent", "Mozilla/5.0");
    req.setRawHeader("Cache-Control", "no-cache");
    req.setRawHeader("Content-Type", "image/png");



    _reply = _nam->get(req);
    connect(_reply, SIGNAL(finished()), this, SLOT(downloadFinished()));

    /*
    QList<QByteArray> headers = req.rawHeaderList();

    for (int i=0; i<headers.size(); i++)
    {
        QString str(headers.at(i));
         LogTrace("request header: %s", str.toStdString().c_str());
    }
    */



// NOTE: Store QNetworkReply pointer (maybe into caller).
// When this HTTP request is finished you will receive this same
// QNetworkReply as response parameter.
// By the QNetworkReply pointer you can identify request and response.


    return true;
}

//============================================================================
//============================================================================
bool BingImg::done() const
{
    return !_querying;
}

//============================================================================
//============================================================================
void BingImg::onDone(PQImage img)
{
    _img = img;
    _querying = false;
}

//============================================================================
//============================================================================
void BingImg::downloadFinished()
{
    const char *func = "BingImg::downloadFinished() - ";

    // Reading attributes of the reply
    // e.g. the HTTP status code
    QVariant statusCodeV = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a redirect:
    QVariant redirectionTargetUrl = _reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to handle this

    QNetworkReply::NetworkError err = _reply->error();

    // no error received?
    if (err == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here

        // Example 1: Creating QImage from the reply
        QImageReader imageReader(_reply);
        PQImage img(new QImage());
        *img = imageReader.read();

        // save to cachefolder
        bool ret = saveToCache(img.get());

        // done
        onDone(img);

        // Example 2: Reading bytes form the reply
        //QByteArray bytes = reply->readAll();  // bytes
        //QString string(bytes); // string
    }
    // Some http error received
    else
    {
        QString url = _reply->url().toString();
        LogError("%s error: %d, msg: downloading aerial image: %s", func, err, _reply->errorString().toStdString().c_str());
        LogError("%s url: %s", func, url.toStdString().c_str() );

        // handle errors here
        onDone(PQImage());
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    _reply->deleteLater();
    _reply = NULL;
}

//============================================================================
//============================================================================
std::string BingImg::createUrlDebug(const Extents &ext)
{
    _ext = ext;
    _ext.makeSquare();

    std::string baseURL= "http://dev.virtualearth.net/REST/v1/Imagery/Map/Aerial";

    // "mapArea=" + str(self.bBox['SouthLatitude']) + "," + str(self.bBox['WestLongitude']) + "," + str(self.bBox['NorthLatitude']) + "," + str(self.bBox['EastLongitude']
    char mapArea[256];
    sprintf(mapArea, "mapArea=%f,%f,%f,%f", _ext.b, _ext.l, _ext.t, _ext.r);
    // http://msdn.microsoft.com/en-us/library/ff701724.aspx
    // Docs say max size is 900, 834 pixels. I can retrieve larger images though, so I'm not quite sure what the actual limit is,
    // or if it is the same for every image. Minimum size is 80,80.
    // Note, change geoReferenceBing if this changes.
    std::string proj = "srs=EPSG:3857";
    std::string mapSize = "mapSize=830,830";
    std::string metaDataRequest = "mapMetadata=1&o=xml";
    std::string mapFormat = "format=png";
    std::string apikey = "AmtfiRWiXQCL5vSmMz0otxtRWEozSxf2PowsOQsRXvTYpKD189BOmg8t-8_3lcJl";

    std::string url = baseURL + "?" + mapArea + "&" + mapSize + "&" + mapFormat + "&" + proj + "&key=" + apikey;

    // debug
    url = "http://dev.virtualearth.net/REST/v1/Imagery/Map/Aerial?mapArea=43.2272179713,-92.9444360738,43.2341907781,-92.9396295552&mapSize=830,830&format=png&key=AmtfiRWiXQCL5vSmMz0otxtRWEozSxf2PowsOQsRXvTYpKD189BOmg8t-8_3lcJl";
    return url;
}


//============================================================================
// example
// http://wms.onterrasystems.com/WMSService.svc/ec65eceb82db4599a0045a5748da8427/WMSLatLon?service=WMS&request=GetMap&version1.1.1&layers=OnTerraWMS&styles=AerialWithLabels&srs=EPSG:3857&format=image/png&width=1024&height=1024&bbox=-10339357.34325100,5353575.60028505,-10338898.38136641,5353926.54532100
//
// http://wms.onterrasystems.com/WMSService.svc/ec65eceb82db4599a0045a5748da8427/WMSLatLon?service=WMS&request=GetMap&version1.1.1&layers=OnTerraWMS&styles=AerialWithLabels&srs=EPSG:3857&format=image/png&width=1024&height=1024&bbox=-11166505.930711,5781352.339324,-11165344.183441,5782514.086595

// http://wms.onterrasystems.com/WMSService.svc/ec65eceb82db4599a0045a5748da8427/WMSLatLon?service=WMS&request=GetMap&version1.1.1&layers=OnTerraWMS&styles=AerialWithLabels&srs=EPSG:3857&format=image/png&width=1024&height=1024&bbox=-1.116651e+007,5.781352e+006,-1.116534e+007,5.782514e+006
//============================================================================
std::string BingImg::createUrlWms()
{
    //const char *func = "BingImg::createUrlWms() - ";

    std::string baseUrl = "http://wms.onterrasystems.com/WMSService.svc/ec65eceb82db4599a0045a5748da8427/WMSLatLon?service=WMS&request=GetMap&version1.1.1&layers=OnTerraWMS";
    std::string style = "styles=AerialWithLabels";
    std::string proj = "srs=EPSG:3857";
    std::string frmt = "format=image/png";
    std::string width = "width=1024";
    std::string height = "height=1024";
    std::string box = UtlString::format("bbox=%.8f,%.8f,%.8f,%.8f", _ext.l, _ext.b, _ext.r, _ext.t); // Note: left, bottom, right, top

    std::string url = baseUrl + "&" + style + "&" + proj + "&" + frmt + "&" + width + "&" + height + "&" + box;
    return url;
}

//============================================================================
//============================================================================
void BingImg::initExtents(const Extents &ext)
{
    _ext = ext;
    _ext.makeSquare();
}


//============================================================================
//============================================================================
bool BingImg::setCachePath(const char *path)
{
   _cacheFolder = path;
   if (_cacheFolder.size() <= 0)
   {
       _cacheFolder = "c:/mapgen/cache/bing/";
   }

   char end = _cacheFolder[_cacheFolder.size()-1];

   if ( end != '/' && end != '\\')
   {
       //_cacheFolder += QDir::separator().toAscii();
       _cacheFolder += QDir::separator().toLatin1(); // for Qt5
   }

   return validateCache();
}

//============================================================================
//============================================================================
bool BingImg::haveCache()
{
    if (!_useCache) return false;

    std::string path = getCachePath(_ext);

    QFile file(path.c_str());
    if (!file.exists()) return false;

    PQImage img(new QImage());
    img->load(path.c_str());
    onDone(img);
    return true;
}

//============================================================================
//============================================================================
bool BingImg::saveToCache(QImage *img)
{
    if (!_useCache) return false;

    std::string path = getCachePath(_ext);
    return img->save(path.c_str());
}

//============================================================================
//============================================================================
std::string BingImg::getCachePath(const Extents &ext) const
{
    // lets round to the nearest meter
    return UtlString::format("%s%.0f_%.0f_%.0f_%.0f.png", _cacheFolder.c_str(), ext.l, ext.b, ext.r, ext.t); // Note: left, bottom, right, top
    //return UtlString::format("%s%.8f_%.8f_%.8f_%.8f.png", _cacheFolder.c_str(), ext.l, ext.b, ext.r, ext.t); // Note: left, bottom, right, top
}

//============================================================================
//============================================================================
bool BingImg::validateCache()
{
    QDir dir(_cacheFolder.c_str());
    if (dir.exists()) return true;

    return dir.mkpath(_cacheFolder.c_str());
}
