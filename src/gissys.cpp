#include "gissys.h"
//#include "toolsGLShader.h"
#include "box3.h"
#include "gldraw.h"
#include "utltxgeo.h"
#include "bingimg.h"
#include "colorpickergradient.h"
#include "geoimg.h"
#include "geoimgraster.h"
#include "geoaerial.h"
#include "geojson.h"
#include "geolayer.h"
#include "sleepsim.h"
#include "utlstring.h"

//============================================================================
//============================================================================
GisSys::GisSys() :
    _fboToDisk(true)
{
    _rootNode.reset(new GlObj());
}

//============================================================================
//============================================================================
PGlObj GisSys::loadTiff(const char *file, PColorPicker picker, Stats *stats, const char *id, bool validation)
{
    const char *func = "GisSys::loadTiff() - ";
    GdalFile tiff;
    if (!tiff.Load(file))
    {
        LogError("%s Failed to load dem tiff: %s", func, file);
        return PGlObj();
    }

    if (picker)
    {
        picker->begin(id);
    }

    PMemBuf tiffdata;
    PTexture tx = UtlTxGeo::loadHmap(&tiff, picker.get(), stats, validation, &tiffdata);

    if (picker)
    {
        picker->end();
    }

    PGlObj img(new GeoImgRaster(tx, *tiff.GetExtents(), picker, *stats, tiffdata));
    //((GeoImgRaster *)img.get())->init(tx, *tiff.GetExtents());
    std::string name = UtlString::getFilename(file);
    name = UtlString::removeExtension(name.c_str());
    img->setName(name);

    return img;
}

//============================================================================
//============================================================================
PGlObj GisSys::loadGeoJson(const char *file, const char *lyrname, const char *objname)
{
    const char *func = "GisSys::loadGeoJson() -";

    // load the json
    GeoJson json;
    if (!json.loadFile(file))
    {
        LogError("%s failed to load json file: %s, for dataobj: %s", func, file, objname);
        return PGlObj();
    }

    PGlObj polys(new GeoObj());
    json.getGeometry(lyrname, polys);
    polys->_name = objname;
    return polys;
}

//============================================================================
//============================================================================
PGlObj GisSys::loadGeoJson(const char *file, int lyrnum, const char *objname)
{
    const char *func = "GisSys::loadGeoJson() -";

    // load the json
    GeoJson json;
    if (!json.loadFile(file))
    {
        LogError("%s failed to load json file: %s, for dataobj: %s", func, file, objname);
        return PGlObj();
    }

    PGlObj polys(new GeoObj());
    json.getGeometry(lyrnum, polys);
    polys->_name = objname;
    return polys;
}

//============================================================================
//============================================================================
PGlObj GisSys::loadDem(const char *file)
{
    const char *func = "GisSys::loadDem() - ";
    GdalFile dem;
    if (!dem.Load(file))
    {
        LogError("%s Failed to load dem tiff: %s", func, file);
        return PGlObj();
    }

    ColorPickerGradient picker(QColor("#55aa00"), QColor("#ffff00"), QColor("#ff0000"));
    PTexture txDemColored = UtlTxGeo::loadHmap(&dem, &picker);

    PGlObj img(new GeoImg());
    ((GeoImg *)img.get())->init(txDemColored, *dem.GetExtents());
    return img;
}

//============================================================================
//============================================================================
PGlObj GisSys::loadChan(const char *file)
{
    const char *func = "GisSys::loadChan() - ";
    GdalFile dem;
    if (!dem.Load(file))
    {
        LogError("%s Failed to load chan tiff: %s", func, file);
        return PGlObj();
    }

    ColorPickerGradient picker(QColor("#ffffff"), QColor("#0000ff"));
    PTexture txDemColored = UtlTxGeo::loadHmap(&dem, &picker);

    PGlObj img(new GeoImg());
    ((GeoImg *)img.get())->init(txDemColored, *dem.GetExtents());
    return img;
}

//============================================================================
//============================================================================
PGlObj GisSys::loadAerial(const Extents &ext)
{
    const char *func = "GisSys::loadAerial() - ";

    GeoAerial *pa = new GeoAerial();
    PGlObj obj(pa);

    if (!loadAerial(pa, ext)) return PGlObj();

    return obj;

    /*
    SleepSim sim;
    BingImg imgbing(m_filePaths->m_pathCache.c_str());
    imgbing.query(ext);
    while (!imgbing.done())
    {
        if (m_app) m_app->processEvents();

        //sim.sleep(10);
    }

    if (!imgbing.getImg().get())
    {
        LogError("%s Failed to get bing aerial imagery with extents: %s", func, ext.asStr().c_str());
        return PGlObj();
    }


    PTexture tx = UtlTxGeo::load(imgbing.getImg().get());
    PGlObj geoimg(new GeoImg(tx, imgbing.getExtents())); // TODO: need to get modified extents from bing here
    return geoimg;
    */
}

//============================================================================
//============================================================================
bool GisSys::loadAerial(GeoAerial *pa, const Extents &ext)
{
    const char *func = "GisSys::loadAerial() - ";

    BingImg imgbing(filePaths()->m_pathCache.c_str());
    imgbing.query(ext);
    while (!imgbing.done())
    {
        if (m_app) m_app->processEvents();

        //sim.sleep(10);
    }

    if (!imgbing.getImg().get())
    {
        LogError("%s Failed to get bing aerial imagery with extents: %s", func, ext.asStr().c_str());
        return false;
    }


    PTexture tx = UtlTxGeo::load(imgbing.getImg().get());
    pa->init(tx, imgbing.getExtents());
    return true;
}

//============================================================================
//============================================================================
bool GisSys::init(QCoreApplication *app, int argc, const char** argv, int winWidth, int winHeight, PFilePaths pPaths)
{
	const char *func = "GisSys::Init() - ";

    if (!GlutSys::init(app, argc, argv, winWidth, winHeight, pPaths))
	{
		LogError("%s GlutSys::Init failed", func);
		return false;
	}

    //if (!initFbo()) return false;

	glDisable(GL_LIGHTING);

    LogTrace(GlutSys::getGLInfo().c_str());
    return true;
}

//============================================================================
//============================================================================
void GisSys::run()
{
    // init fbo if not already
    if (!_fbo)
    {
        initFbo();
    }

    GlutSys::run();
}

//============================================================================
//============================================================================
bool GisSys::haveData()
{
    if (_rootNode == NULL || _rootNode->childList().size() <= 0)
    {
        return false;
    }

    return true;
}

//============================================================================
//============================================================================
bool GisSys::setConfig(PConfig cfg)
{
    m_dd->_cfg = cfg;
    if (!initFbo()) return false;

    /*
    if (!_cfg) return true;
    if (_cfg->windowless())
    {
        glutHideWindow();
    }
    */

    const Rgbf& c = cfg->colrClear();
    glClearColor(c.GetR(), c.GetG(), c.GetB(), 1.0);
    

    return true;
}

//============================================================================
//============================================================================
Config* GisSys::getConfig()
{
    return m_dd->_cfg.get();
}

//============================================================================
//============================================================================
void GisSys::saveFrameToDisk(const std::string &path)
{
    _fboToDisk = true;
    _fbo->getDrawParams()->saveScreenShot = true;
    _fbo->getDrawParams()->pathScreenShot = path;

}

//============================================================================
//============================================================================
void GisSys::zoomExtents(double incper)
{
    box3d bbox = computeBox();
    LogTrace("Map bounding box: L: %f, R: %f, T: %f, B: %f, W: %f, H: %f", bbox.vmin.x, bbox.vmax.x, bbox.vmax.y, bbox.vmin.y, bbox.getWidth(), bbox.getHeight());

	double xinc = ((bbox.vmax.x - bbox.vmin.x) * incper) / 2.0;
	double yinc = ((bbox.vmax.y - bbox.vmin.y) * incper) / 2.0;
    Extents ext(bbox.vmin.x - xinc, bbox.vmax.y + yinc, bbox.vmax.x + xinc, bbox.vmin.y - yinc);
    zoomExtents(ext);
}

//============================================================================
//============================================================================
void GisSys::zoomExtents(const Extents &ext)
{
    moveSys()->m_camera->setExtents2d(ext, true);
    //m_moveSys->m_camera->SetUpOrthoZoom(m_winW, m_winH, ext);

    if (_fbo)
    {
        _fbo->setCameraExtents2d(ext, false);
    }

    if (_fboComp)
    {
        _fboComp->setCameraExtents2d(ext, false);
    }
}

//============================================================================
//============================================================================
void GisSys::insertBack(PGlObj obj)
{
    _rootNode->insertBack(obj);
}

//============================================================================
//============================================================================
void GisSys::insertFront(PGlObj obj)
{
    _rootNode->insertFront(obj);
}

//============================================================================
//============================================================================
void GisSys::insert(GlObjList *list)
{
    if (!list) return;

    for (unsigned int i=0; i<list->size(); i++)
    {
        insertBack((*list)[i]);
    }
}

//============================================================================
//============================================================================
void GisSys::initObjs()
{
    if (_rootNode) _rootNode->init(m_dd.get());
}

//============================================================================
//============================================================================
void GisSys::clearObjs()
{
    _rootNode->clearChildren();
}

//============================================================================
//============================================================================
box3d GisSys::computeBox()
{
    box3d bbox;
    _rootNode->computeBox(m_dd.get(), &bbox);
    return bbox;
}

//============================================================================
//============================================================================
void GisSys::onPreDisplay()
{
    if (!_batchCfg.get()) return;

    if (!_batchCfg->process(this))
    {
        if (m_dd->_cfg && m_dd->_cfg->windowless())
        {
            exit(); // done
        }
    }
}

//============================================================================
//============================================================================
void GisSys::displayData()
{
    GlutSys::displayData();

    if (m_dd->_cfg && m_dd->_cfg->lyrOutMode())
    {
        displayLyrOutMode();
    }
    else
    {
        displayStandard();
    }
    
    if (dataSel()->sel())
    {
        dataDraw()->_drawSelected = true;
        dataSel()->sel()->draw(dataDraw());
        dataDraw()->_drawSelected = false;
    }

    // only exit if there is no batch config, if there is one we want to keep running
    if (!_batchCfg)
    {
        if (m_dd->_cfg && m_dd->_cfg->windowless())
        {
            exit(); // done
        }
    }

    dataValidate()->validate(false);
}

//============================================================================
//============================================================================
void GisSys::displayStandard()
{
    if (!_rootNode) return;

    _rootNode->draw(dataDraw());

    if (_fboToDisk && _fbo)
    {
        _fbo->draw(_rootNode.get(), m_dd.get());
        /*
        _fbo->drawStart();
        _rootNode->draw(m_dd.get());
        _fbo->drawEnd();
        */
        _fboToDisk = false;
    }
}

//============================================================================
//============================================================================
void GisSys::displayLyrOutMode()
{
    if (!_rootNode) return;

    GlObjList lyrList;
    GlObjList &objList = _rootNode->childList();
    GlObjList::iterator it = objList.begin();

    /*
    if (_fboComp)
    {
        _fboComp->updateDrawParams();
        _fboComp->getFboCb()->drawStart(true, true, true);
    }
    */

    int lyrnum = 0;
    while (it != objList.end())
    {
        PGlObj obj = *it;
        it++;

        GeoLayer *lyr = dynamic_cast<GeoLayer *>(obj.get());
        if (!lyr) continue;

        displayLyrOut(lyr, lyrnum);
        lyrnum++;
        lyrList.push_back(obj);
    }

    /*
    if (_fboComp)
    {
        // this layer by layer composite doesn't look as good and a rendered composite
        _fboComp->getFboCb()->getDrawParams()->saveFrame = false;
        _fboComp->getFboCb()->getDrawParams()->saveScreenShot = false;
        _fboComp->getFboCb()->drawEnd();

        // render a composite
        if (_fboToDisk)
        {
            std::string fullpath = m_dd->_cfg->imgFolder() + std::string("composite.png");
            _fboComp->getFboCb()->saveFrame(fullpath.c_str());
        }

        _fboComp->blitToScreen(m_dd.get());
    }
    */
    
    if (_fbo)
    {
        _fbo->getDrawParams()->saveScreenShot = _fboToDisk;
        if (_fboToDisk)
        {
            std::string fullpath = m_dd->_cfg->imgFolder() + std::string("composite.png");
            _fbo->getDrawParams()->pathScreenShot = fullpath;
        }


        GlObj robj;
        robj.childList().insert(robj.childList().end(), lyrList.begin(), lyrList.end());

        float alphaPrev = _fbo->getDrawParams()->colr[3];
        _fbo->getDrawParams()->colr[3] = 1; // turn off alpha background for the composite;
        _fbo->draw(&robj, m_dd.get(), FboRender::E_RF_OVERRIDE_NONE, true);
        _fbo->getDrawParams()->colr[3] = alphaPrev;
    }
    


    _fboToDisk = false;
}

//============================================================================
//============================================================================
void GisSys::displayLyrOut(GeoLayer *lyr, int lyrnum)
{
    // lyr->draw(m_dd.get());

    if (!_fbo) return;
    //if (!_fboToDisk || !_fbo) return;
    
    if (_fboToDisk)
    {
        std::string name = lyr->getName();

        if (name.size() <= 0) name = to_string(lyrnum);
        name += ".png";

        std::string fullpath = m_dd->_cfg->imgFolder() + name;

        _fbo->getDrawParams()->pathScreenShot = fullpath;
    }
    else
    {
        _fbo->getDrawParams()->saveScreenShot = false;
    }

    _fbo->draw(lyr, m_dd.get(), lyr->renderFlags());

    /*
    if (_fboComp)
    {
        Rect2d rcDstTot(0, _fboComp->getFboCb()->getW(), _fboComp->getFboCb()->getH(), 0);
        _fbo->getFboCb()->renderThis(rcDstTot, _fboComp->getFboCb()->getFboId());
    }
    */
}

//============================================================================
//============================================================================
void GisSys::setCamera()
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//============================================================================
//============================================================================
void GisSys::destroy()
{
    _fbo.reset();
    _batchCfg.reset();
    _rootNode.reset();

    GlutSys::destroy();
}

//============================================================================
//============================================================================
void GisSys::onKeyboard(unsigned char key, int x, int y)
{
    GlutSys::onKeyboard(key, x, y);
}

//============================================================================
//============================================================================
void GisSys::onMouseMove()
{
    GlutSys::onMouseMove();

    dataSel()->init(dataMouse()->x(), dataMouse()->y(), true);
    _rootNode->runSel(m_dd.get());
}

//============================================================================
//============================================================================
bool GisSys::initFbo()
{
    const char *func = "GisSys::initFbo() - ";

    if (!m_dd->_cfg)
    {
        _fbo.reset();
        return false;
    }

    PCamera camera(new Camera(Camera::Ortho));
    camera->onResize(m_dd->_cfg->width(), m_dd->_cfg->height());

    uint32_t renderFlags = FboRender::E_RF_NONE;
    GLsizei msaaSamps = m_dd->_cfg->msaaSamples();
    GLsizei jtaaSamps = m_dd->_cfg->jtaaSamples();
    double jtaaOfsPixels = m_dd->_cfg->jtaaOffset();
    GLsizei ssaaMul = m_dd->_cfg->ssaaMul();

    if (m_dd->_cfg->ssaaOn())
    {
        renderFlags |= FboRender::E_RF_SUPERSAMPLE;
    }
    if (m_dd->_cfg->jtaaOn())
    {
        renderFlags |= FboRender::E_RF_JITTER;
    }
    if (m_dd->_cfg->msaaOn())
    {
        renderFlags |= FboRender::E_RF_MULTISAMPLE;
    }

    _fbo.reset(new FboRender());
    if (!_fbo->create(camera, m_dd->_cfg->width(), m_dd->_cfg->height(), renderFlags, msaaSamps, ssaaMul, jtaaSamps, jtaaOfsPixels))
    {
        LogError("%s fbo creation failed", func);
        return false;
    }

    _fboComp.reset(new FboRender());
    if (!_fboComp->create(camera, m_dd->_cfg->width(), m_dd->_cfg->height(), FboRender::E_RF_NONE))
    {
        LogError("%s fbo composite creation failed", func);
        return false;
    }

    const Rgbf& c = m_dd->_cfg->colrClear();
    _fbo->getDrawParams()->saveFrame = false;
    _fbo->getDrawParams()->saveScreenShot = m_dd->_cfg->toDisk();
    _fbo->getDrawParams()->pathScreenShot = m_dd->_cfg->imgFile(); //m_filePaths->m_pathImgOut + "map.png";
    _fbo->getDrawParams()->colr[0] = c.GetR();
    _fbo->getDrawParams()->colr[1] = c.GetG();
    _fbo->getDrawParams()->colr[2] = c.GetB();
    _fbo->getDrawParams()->colr[3] = c.GetA();

    _fboComp->getDrawParams()->saveFrame = false;
    _fboComp->getDrawParams()->saveScreenShot = m_dd->_cfg->toDisk();
    _fboComp->getDrawParams()->pathScreenShot = m_dd->_cfg->imgFile(); //m_filePaths->m_pathImgOut + "map.png";
    _fboComp->getDrawParams()->colr[0] = c.GetR();
    _fboComp->getDrawParams()->colr[1] = c.GetG();
    _fboComp->getDrawParams()->colr[2] = c.GetB();
    _fboComp->getDrawParams()->colr[3] = c.GetA();
    //m_fbo->getDrawParams()->znr = m_pXmlGeo->ZNear();
    //m_fbo->getDrawParams()->zfr = m_pXmlGeo->ZFar();
    return true;
}
