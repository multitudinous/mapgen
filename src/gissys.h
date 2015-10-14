#ifndef GISSYS_H
#define GISSYS_H

#include "sysdefs.h"
#include "glutsys.h"
#include "textures.h"
#include "fborender.h"
#include "globj.h"
#include "batchcfg.h"
#include "gradientpicker.h"

class GeoAerial;
class GeoLayer;
class Stats;

class GisSys : protected GlutSys
{
public:
	GisSys();

    virtual bool init(QCoreApplication *app, int argc, const char** argv, int winWidth, int winHeight, PFilePaths pPaths);
    virtual void run();

    bool haveData();

    bool setConfig(PConfig cfg);
    Config* getConfig();

    void setBatchCfg(PBatchCfg cfg) { _batchCfg = cfg; }

    void saveFrameToDisk(const std::string &path);

    void zoomExtents();
    void zoomExtents(const Extents &ext);

    void insertBack(PGlObj obj);
    void insertFront(PGlObj obj);
    void insert(GlObjList *list);
    void clearObjs();
    void initObjs();

    box3d computeBox();

    virtual DrawData* dd() { return m_dd.get(); }

    static PGlObj loadTiff(const char *file, PColorPicker picker, Stats *stats, const char *id = "id", bool validation=false);
    static PGlObj loadGeoJson(const char *file, const char *lyrname, const char *objname="");
    static PGlObj loadGeoJson(const char *file, int lyrnum, const char *objname="");

    PGlObj loadDem(const char *file);
    PGlObj loadChan(const char *file);
    PGlObj loadAerial(const Extents &ext);
    bool loadAerial(GeoAerial *pa, const Extents &ext);

protected:
    virtual void onPreDisplay();
    virtual void displayData();
    virtual void setCamera();

    virtual void destroy();
    virtual void onKeyboard(unsigned char key, int x, int y);

    bool initFbo();

    void displayStandard();
    void displayLyrOutMode();
    void displayLyrOut(GeoLayer *lyr, int lyrnum);


protected:
    PBatchCfg _batchCfg;
    PGlObj _rootNode;
    PFboRender _fbo;
    PFboRender _fboComp;
    bool _fboToDisk;

    //PConfig _cfg;
};

#endif
