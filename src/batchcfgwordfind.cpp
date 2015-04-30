#include "batchcfgwordfind.h"
#include "gissys.h"
#include "utlqt.h"
#include <QString>
#include <QDateTime>
#include "geocompute.h"
#include "geowords.h"
#include "geopoly.h"
#include "geolayer.h"

//============================================================================
//============================================================================
BatchCfgWordFind::BatchCfgWordFind(const char *jsonFolder, const char *wordFolder, const char *outFolder) : BatchCfg(),
    _jsonFolder(jsonFolder),
    _wordFolder(wordFolder),
    _outFolder(outFolder),
    _mapStyle(new DrawAttr())
{


    _mapStyle->_name = "fieldstyle";

    _mapStyle->_drawPolyOutline = true;
    _mapStyle->_colorPolyOutline = Rgbf(0,0,0,1);
    _mapStyle->_drawLabels = true;
    _mapStyle->_colorLabels = Rgbf(0,0,0,1);
    _mapStyle->_feature = "section";
    _mapStyle->_font.initFontTexture("C:/Windows/Fonts/ARIAL.TTF", 12); // TODO: figure out how to configure fonts folder, and search path
}

//============================================================================
//============================================================================
bool BatchCfgWordFind::init()
{
    const char *func = "BatchCfgWordFind::init() -";

    _batchItems.clear();

    if (!UtlQt::validateDir(_jsonFolder.c_str(), false))
    {
        LogError("%s json folder is not valid: %s", func, _jsonFolder.c_str());
        return false;
    }

    if (!UtlQt::validateDir(_wordFolder.c_str(), false))
    {
        LogError("%s word folder is not valid: %s", func, _wordFolder.c_str());
        return false;
    }

    if (!UtlQt::validateDir(_outFolder.c_str(), true))
    {
        LogError("%s output folder is not valid: %s", func, _outFolder.c_str());
        return false;
    }

    // validation name

    QString vdt = UtlQt::getDateTimeYMDHMS() + QString("_geowordvalidation.txt");
    std::string valfile = UtlQt::pathCombine(QString(_outFolder.c_str()), vdt).toStdString();

    // find all json files
    std::vector<QString> jsonList;
    UtlQt::findFiles(QString(_jsonFolder.c_str()), QString("*.json"), &jsonList, false);
    UtlQt::findFiles(QString(_jsonFolder.c_str()), QString("*.geojson"), &jsonList, false);

    // find all word files
    std::list<QString> wordList;
    UtlQt::findFiles(QString(_wordFolder.c_str()), QString("*.txt"), &wordList, false);

    // match all json files with word files
    for (size_t i=0; i<jsonList.size(); i++)
    {
        QFileInfo info(jsonList[i]);
        QString jsonName = info.baseName();

        bool found = false;
        std::list<QString>::iterator it = wordList.begin();
        while (it != wordList.end())
        {
            QFileInfo info(*it);
            QString wordName = info.baseName();

            if (QString::compare(jsonName, wordName, Qt::CaseInsensitive))
            {
                it++;
                continue;
            }

            // we found a match

            PBatchItem item( new BatchItem());
            item->jsonFile = jsonList[i].toStdString();
            item->wordFile = (*it).toStdString();
            item->outImgFile = UtlQt::pathCombine(QString(_outFolder.c_str()), QString(wordName + ".png")).toStdString();
            item->resultsFile = UtlQt::pathCombine(QString(_outFolder.c_str()), QString(wordName + "_geowordmatch.txt")).toStdString();
            item->validationFile = valfile;
            item->name = wordName.toStdString();
            _batchItems.push_back(item);

            // remove this word file since its been matched
            wordList.erase(it);
            found = true;
            break;
        }

        if (!found)
        {
            LogError("%s failed to find a matching word file for json file %s, search folder: %s", func, jsonList[i].toStdString().c_str(), _wordFolder.c_str());
        }
    }

    if (_batchItems.size() <= 0)
    {
        LogError("%s failed to find any batch items to process.", func);
        return false;
    }

    LogTrace("%s found %d items to process.", func, _batchItems.size());

    return true;
}

//============================================================================
//============================================================================
bool BatchCfgWordFind::process(GisSys *pgis)
{
    const char *func = "BatchCfgWordFind::process() -";

    if (_batchItems.size() <= 0) return false; // all done
    if (!pgis) return false;

    while (_batchItems.size() > 0)
    {
        PBatchItem item = _batchItems.front();
        _batchItems.pop_front();


        pgis->getConfig()->imgFile(item->outImgFile);
        pgis->clearObjs();

        // load json
        PGlObj polys = GisSys::loadGeoJson(item->jsonFile.c_str(), "fielddata");
        if (!polys.get())
        {
            LogError("%s skipping file: %s due to load failure", func, item->jsonFile.c_str());
            continue;
        }

        // load words
        GeoWords *pwords = new GeoWords();
        PGlObj words(pwords);
        pwords->_name = "words";
        pwords->setComputeBoxMode(GeoWords::BoxCompute_Auto);
        if (!pwords->load(item->wordFile.c_str()))
        {
            LogError("%s skipping file: %s due to load failure", func, item->wordFile.c_str());
            continue;
        }


        // create compute object
        GeoCompute *pcompute(new GeoCompute());
        PGlObj compute(pcompute);

        pcompute->setName(item->name);
        pcompute->setType("wordfinder");
        pcompute->setFeature("section");
        pcompute->setOutput(item->resultsFile);
        pcompute->setValidationFile(item->validationFile);

        pcompute->insertBack(polys);
        pcompute->insertBack(words);

        // create a layer
        GeoLayer *plyr = new GeoLayer();
        PGlObj lyr(plyr);

        plyr->_name = "PdfField";
        plyr->setDrawAttr(_mapStyle);
        plyr->insertBack(compute);

        // run the compute
        LogTrace("running compute on json %s and words %s ...", item->jsonFile.c_str(), item->wordFile.c_str());
        if (!pcompute->compute())
        {
            LogError("%s compute failed on json %s and words %s", func, item->jsonFile.c_str(), item->wordFile.c_str());
            continue;
        }

        // lets render the compute layer map
        pgis->insertBack(lyr);
        pgis->initObjs();
        pgis->zoomExtents();
        pgis->saveFrameToDisk(item->outImgFile);


        LogTrace("%s there are %d items left to process", func, _batchItems.size());
        return true;
    }

    return false; // done
}
