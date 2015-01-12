#include <QtCore/QCoreApplication>
#include "sysdefs.h"
#include "gdalFile.h"
#include "ogrsf_frmts.h"
#include "gissys.h"
#include "geojson.h"
#include "geolayer.h"
#include "bingimg.h"
#include "mapyaml.h"
#include "batchcfgwordfind.h"

void initYaml(const char *file, GisSys *sys);
bool initBatchWordFind(int argc, char *argv[], GisSys *sys);

void initTestSci(GisSys *sys);
void initTestSciDemMask(GisSys *sys);
void initAerialDemMask(GisSys *sys);

//============================================================================
//============================================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   //return a.exec();


    GDALAllRegister();
    OGRRegisterAll();
    GDALRegister_WMS();

    PFilePaths filePaths(new FilePaths());
    filePaths->Init(argv[0]);

    GisSys sys;
    if (!sys.init(&a, argc, (const char **)argv, 800, 600, filePaths)) return -1;

   // GdalFile file;
    //file.WmsTest();

    //initTestSci(&sys);
    //initTestSciDemMask(&sys);
    //initAerialDemMask(&sys);
    if (argc < 2)
    {
        LogError("usage yamlfilepath or --batchwordfind jsonfolder wordfolder outfolder");
        return 0;
    }
    else if (!strcmp(argv[1], "--batchwordfind"))
    {
        if (!initBatchWordFind(argc, argv, &sys))
        {
            return 0;
        }
    }
    else
    {
        initYaml(argv[1], &sys);
    }

    sys.run(&a);

    return 0;
}

//============================================================================
//============================================================================
void initYaml(const char *file, GisSys *sys)
{
    MapYaml yaml;
    yaml.load(file, sys);
    //yaml.load("D:/dev/gis/test/aerial/yaml.txt", sys);
}

//============================================================================
//============================================================================
bool initBatchWordFind(int argc, char *argv[], GisSys *sys)
{
    if (argc < 5)
    {
        LogError("usage --batchwordfind jsonfolder wordfolder outfolder");
        return false;
    }

    PBatchCfg cfg(new BatchCfgWordFind(argv[2], argv[3], argv[4]));
    if (!cfg->init())
    {
        LogError("batchwordfind init failed, unable to process.");
        return false;
    }

    sys->setBatchCfg(cfg);
    return true;
}

//============================================================================
//============================================================================
void initTestSci(GisSys *sys)
{
    GeoJson json;
    json.loadFile("D:/dev/gis/test/input.geojson");

    PGlObj lyrPolysFill(new GeoLayer());
    json.getGeometry(lyrPolysFill);

    PGlObj lyrLabels(new GeoLayer());
    lyrLabels->shallowCopy(lyrPolysFill.get());

    PDrawAttr attrFill(new DrawAttr());
    attrFill->_drawLabels = false;
    //attr->_feature = "sci"; //"musym";
    attrFill->_drawPolyFill = true;
    attrFill->_drawPolyOutline = true;
    attrFill->_colorPolyFill[0] = 142.0f/255.0f;
    attrFill->_colorPolyFill[1] = 237.0f/255.0f;
    attrFill->_colorPolyFill[2] = 58.0f/255.0f;

    attrFill->_colorPolyOutline[0] = 0;
    attrFill->_colorPolyOutline[1] = 0;
    attrFill->_colorPolyOutline[2] = 0;



    //attr->_font.initFontTexture("C:/Windows/Fonts/BERNHC.TTF", 24);
    //attr->_font.initFontTexture("C:/Windows/Fonts/ARIAL.TTF", 36);
    lyrPolysFill->setDrawAttr(attrFill);
    sys->insertBack(lyrPolysFill);

    PDrawAttr attrLabl(new DrawAttr());
    attrLabl->_font.initFontTexture("C:/Windows/Fonts/ARIAL.TTF", 36);
    attrLabl->_drawLabels = true;
    attrLabl->_feature = "sci"; //"musym";
    attrLabl->_drawPolyFill = false;
    attrLabl->_drawPolyOutline = false;

    attrLabl->_colorLabels[0] = 0;
    attrLabl->_colorLabels[1] = 0;
    attrLabl->_colorLabels[2] = 0;
    lyrLabels->setDrawAttr(attrLabl);
    lyrLabels->initLabels(sys->dd());
    sys->insertBack(lyrLabels);

    sys->zoomExtents();

    // test bing
    box3d bbox = sys->computeBox();
    Extents ext(bbox.vmin.x, bbox.vmax.y, bbox.vmax.x, bbox.vmin.y);

    GdalFile file;
    file.LoadBingWms(ext);
}

//============================================================================
//============================================================================
void initTestSciDemMask(GisSys *sys)
{
    PGlObj mask = sys->loadDem("D:/dev/gis/test/aae80f3c-93da-4fee-a986-596f00fb1505_11581d06-4620-45fd-9401-bc51f06decb0_dem.tif");//"D:/dev/gis/test/dem.tif");
    if (!mask) return;

    GeoJson json;
    //json.loadFile("D:/dev/gis/test/input.geojson");
    json.loadFile("D:/dev/gis/test/aae80f3c-93da-4fee-a986-596f00fb1505_11581d06-4620-45fd-9401-bc51f06decb0_fieldboundary.json");

    PGlObj lyrPolysFill(new GeoLayer());
    json.getGeometry(lyrPolysFill);

    /*
    PGlObj lyrLabels(new GeoLayer());
    lyrLabels->shallowCopy(lyrPolysFill.get());
    */

    PDrawAttr attrFill(new DrawAttr());
    attrFill->_drawLabels = false;
    //attr->_feature = "sci"; //"musym";
    //attrFill->_drawPolyMasked = true;
    attrFill->_drawPolyFill = false;
    attrFill->_drawPolyOutline = true;
    attrFill->_colorPolyFill[0] = 142.0f/255.0f;
    attrFill->_colorPolyFill[1] = 237.0f/255.0f;
    attrFill->_colorPolyFill[2] = 58.0f/255.0f;

    attrFill->_maskPolyList.push_back(mask);

    attrFill->_colorPolyOutline[0] = 0;
    attrFill->_colorPolyOutline[1] = 0;
    attrFill->_colorPolyOutline[2] = 0;

    lyrPolysFill->setDrawAttr(attrFill);
    sys->insertBack(lyrPolysFill);

    /*
    PDrawAttr attrLabl(new DrawAttr());
    attrLabl->_font.initFontTexture("C:/Windows/Fonts/ARIAL.TTF", 36);
    attrLabl->_drawLabels = true;
    attrLabl->_feature = "sci"; //"musym";
    attrLabl->_drawPolyFill = false;
    attrLabl->_drawPolyOutline = false;

    attrLabl->_colorLabels[0] = 0;
    attrLabl->_colorLabels[1] = 0;
    attrLabl->_colorLabels[2] = 0;
    lyrLabels->setDrawAttr(attrLabl);
    lyrLabels->initLabels(sys->dd());
    sys->insertObj(lyrLabels);
    */


    //sys->insertObj(mask);

    sys->zoomExtents();

    // test wms
    box3d bbox = sys->computeBox();
    Extents ext(bbox.vmin.x, bbox.vmax.y, bbox.vmax.x, bbox.vmin.y);
    ext.expand(500);
    //ext = ext * 1.08;

    //sys->clearObjs();

    PGlObj aerial =  sys->loadAerial(ext);
    if (aerial) sys->insertFront(aerial);

    sys->zoomExtents();
}

//============================================================================
//============================================================================
void initAerialDemMask(GisSys *sys)
{
    PGlObj maskdem = sys->loadDem("D:/dev/gis/test/aerial/dem.tif");//"D:/dev/gis/test/dem.tif");
    if (!maskdem) return;

    // load channel
    PGlObj maskchan = sys->loadChan("D:/dev/gis/test/aerial/chan.tif");
    if (!maskchan) return;

    GeoJson json;
    //json.loadFile("D:/dev/gis/test/input.geojson");
    json.loadFile("D:/dev/gis/test/aerial/bnd.json");

    PGlObj lyrBnd(new GeoLayer());
    json.getGeometry(lyrBnd);

    PDrawAttr attrFill(new DrawAttr());
    attrFill->_drawLabels = false;
    //attr->_feature = "sci"; //"musym";
    attrFill->_drawPolyMasked = true;
    attrFill->_drawPolyFill = false;
    attrFill->_drawPolyOutline = true;
    attrFill->_colorPolyFill[0] = 142.0f/255.0f;
    attrFill->_colorPolyFill[1] = 237.0f/255.0f;
    attrFill->_colorPolyFill[2] = 58.0f/255.0f;

    attrFill->_maskPolyList.push_back(maskdem);
    attrFill->_maskPolyList.push_back(maskchan);

    attrFill->_colorPolyOutline[0] = 0;
    attrFill->_colorPolyOutline[1] = 0;
    attrFill->_colorPolyOutline[2] = 1;

    // insert masked boundary
    lyrBnd->setDrawAttr(attrFill);
    sys->insertBack(lyrBnd);

    // name label
    PGlObj lyrLabels(new GeoLayer());
    lyrLabels->shallowCopy(lyrBnd.get());

    PDrawAttr attrLabl(new DrawAttr());
    attrLabl->_font.initFontTexture("C:/Windows/Fonts/ARIAL.TTF", 72);
    attrLabl->_drawLabels = true;
    attrLabl->_feature = "name"; //"musym";
    attrLabl->_drawPolyFill = false;
    attrLabl->_drawPolyOutline = false;

    attrLabl->_useLabelPosOffset = true;
    attrLabl->_labelOfs.dX = 0;
    attrLabl->_labelOfs.dY = 50;

    attrLabl->_colorLabels[0] = 0;
    attrLabl->_colorLabels[1] = 0;
    attrLabl->_colorLabels[2] = 1;
    lyrLabels->setDrawAttr(attrLabl);
    lyrLabels->initLabels(sys->dd());
    sys->insertBack(lyrLabels);


    //sys->insertObj(mask);

    sys->zoomExtents();

    // test wms
    box3d bbox = sys->computeBox();
    Extents ext(bbox.vmin.x, bbox.vmax.y, bbox.vmax.x, bbox.vmin.y);
    ext.expand(500);
    //ext = ext * 1.08;

    //sys->clearObjs();

    PGlObj aerial =  sys->loadAerial(ext);
    if (aerial) sys->insertFront(aerial); // insert to front so draw first

    sys->zoomExtents();
}

/*
#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
void QTest::qSleep(int ms)
{
    QTEST_ASSERT(ms > 0);

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}
*/

