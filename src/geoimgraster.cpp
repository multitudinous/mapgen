#include "geoimgraster.h"
#include "utlmath.h"
#include "utlgl.h"
#include "geotext.h"
#include "utlstring.h"
#include "utlqt.h"
#include "fbo.h"
#include <QImage>

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster() :
_cselimg({0,0,0,0}),
_cselscr({ 0,0,0,0}),
_cselgrd({ 0, 0, 0, 0 }),
_seldatav(0),
_selmx(0),
_selmy(0),
_seltext(true)
{
    for (int i = 0; i < 4; i++)
    {
        _cselimg[i] = 0;
        _cselscr[i] = 0;
    }
}

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster(PTexture tx, const Extents &ext) : 
GeoImg(tx, ext),
_cselimg({ 0, 0, 0, 0 }),
_cselscr({ 0, 0, 0, 0 }),
_cselgrd({ 0, 0, 0, 0 }),
_seldatav(0),
_selmx(0),
_selmy(0),
_seltext(true)
{
}

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster(PTexture tx, const Extents &ext, PColorPicker picker, const Stats &stats, PMemBuf dataValues) :
GeoImg(tx, ext),
_picker(picker),
_stats(stats),
_dataValues(dataValues),
_cselimg({ 0, 0, 0, 0 }),
_cselscr({ 0, 0, 0, 0 }),
_cselgrd({ 0, 0, 0, 0 }),
_seldatav(0),
_selmx(0),
_selmy(0),
_seltext(true)
{
}


//============================================================================
//============================================================================
void GeoImgRaster::draw(DrawData *pdd)
{
    GeoImg::draw(pdd);

    // run a validation routine
    if (pdd->dataValidate()->validate())
    {
        drawValidate(pdd, pdd->dataValidate()->file());
    }

    // is this selected
    if (!pdd->_drawSelected) return;

    if (!pdd->_drawAttrOverlay) return; // no attribute to draw with
    if (_seltext.text().size() <= 0) return;


        // lets draw our results as a 2d overlay
    UtlGL::overaly2dInit();
    
    PDrawAttr attrprev = pdd->_drawAttr;
    pdd->_drawAttr = pdd->_drawAttrOverlay;
    _seltext.draw(pdd);
    pdd->_drawAttr = attrprev;

    UtlGL::overaly2dRestore();
}

//============================================================================
//============================================================================
void GeoImgRaster::runSel(DrawData *pdd)
{
    GeoObj::runSel(pdd);

    _seltext.text(""); // clear the text
    if (!_tx) return;
    if (!_tx->img()) return;
    if (!_dataValues) return;

    // select in 2d
    if (pdd->dataSel()->sray().x < _ext.l) return;
    if (pdd->dataSel()->sray().x > _ext.r) return;
    if (pdd->dataSel()->sray().y > _ext.t) return;
    if (pdd->dataSel()->sray().y < _ext.b) return;

    // we are within the rectangle, find normalized coordinates
    double worldx = pdd->dataSel()->sray().x;
    double worldy = pdd->dataSel()->sray().y;
    double u = (worldx - _ext.l) / _ext.width();
    double v = (worldy - _ext.b) / _ext.height();

    v = UtlMath::clamp(v, 0.0, 1.0);
    u = UtlMath::clamp(u, 0.0, 1.0);

    // first byte of image data is v = 0
    // image is being flipped vertically (see drawing routine in GeoImg), so adjust v for our memory lookup
    v = 1.0 - v;

    dataValidate(u, v, &_seldatav, &_seldatano, &_seldatana, &_cselimg, &_cselgrd, &_cselscr, pdd->dataSel()->mx(), pdd->dataSel()->my());

    /*
    // now we can find the pixel color value in the image
    int bytesperpix = _tx->img()->getSize();
    int w = _tx->img()->getLenX();
    int h = _tx->img()->getLenY();

    int py = v * (h - 1); // get to the correct row
    int px = u * (w - 1); // get to the correct col
    int loc = (py * bytesperpix * w) + (px * bytesperpix);

    if (loc + bytesperpix >= _tx->img()->getByteCount())
    {
        return; // unexpected, out of bounds
    }

    BYTE *buf = (BYTE *)_tx->img()->getBuf();
    _cselimg = { 0, 0, 0, 255 };
    _cselimg[0] = buf[loc+0];
    if (bytesperpix >= 3)
    {
        _cselimg[1] = buf[loc + 1];
        _cselimg[2] = buf[loc + 2];
        if (bytesperpix == 4)
        {
            _cselimg[3] = buf[loc + 3];
        }
    }

    // if zero alpha then no data
    if (_cselimg[3] == 0) return;

    // find the data value
    double *pdata = (double*)_dataValues->getBuf();
    _seldatav = pdata[py * w + px];

    // compute normalized value
    _seldatana = _stats.getPosNorm(_seldatav);
    _seldatano = _stats.getPosNormOrig(_seldatav);

    // get the screen color
    _cselscr = { 0, 0, 0, 255 };
    glReadPixels(pdd->dataSel()->mx(), pdd->dataSel()->my(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &_cselscr[0]);

    // get the gradient color
    QColor c = _picker->pickByPercent(_seldatana);
    _cselgrd[0] = c.red();
    _cselgrd[1] = c.green();
    _cselgrd[2] = c.blue();
    _cselgrd[3] = c.alpha();
    */
    


    _selmx = pdd->dataSel()->mx();
    _selmy = pdd->dataSel()->my();

    // create info block text
    std::string info;
    createInfoBlock(_seldatav, _seldatano, _seldatana, _cselimg, _cselscr, _cselgrd, &info);

    /*
    std::string strimgc, strscrc, strgrdc, strstat, strdata, str;
    UtlString::format(strimgc, "ImgC: %d,%d,%d,%d", _cselimg[0], _cselimg[1], _cselimg[2], _cselimg[3]);
    UtlString::format(strscrc, "ScrC: %d,%d,%d,%d", _cselscr[0], _cselscr[1], _cselscr[2], _cselscr[3]);
    UtlString::format(strgrdc, "GrdC: %d,%d,%d,%d", _cselgrd[0], _cselgrd[1], _cselgrd[2], _cselgrd[3]);

    if (_stats.getForceMin() == Stats::FORCE_MINMAX_OFF && _stats.getForceMax() == Stats::FORCE_MINMAX_OFF)
    {
        UtlString::format(strstat, "amin: %.2f, amax: %.2f, mean: %.2f\nomin:%.2f, omax:%.2f\nsdevs: %.2f",
            _stats.getMin(), _stats.getMax(), _stats.getMean(),
            _stats.getMinOrig(), _stats.getMaxOrig(),
            _stats.getForceSdevs());
    }
    else
    {
        UtlString::format(strstat, "amin: %.2f, amax: %.2f, mean: %.2f\nomin:%.2f, omax:%.2f\nsdevs: %.2f, fmin: %.2f, fmax: %.2f",
            _stats.getMin(), _stats.getMax(), _stats.getMean(),
            _stats.getMinOrig(), _stats.getMaxOrig(),
            _stats.getForceSdevs(), _stats.getForceMin(), _stats.getForceMax());
    }
    UtlString::format(strdata, "Data Value: %.2f, NormO: %.2f, NormA: %.2f", _seldatav, _seldatano, _seldatana);

    UtlString::format(str, "%s\n%s\n%s\n%s\n%s", strdata.c_str(), strstat.c_str(), strimgc.c_str(), strscrc.c_str(), strgrdc.c_str());
    */
    _seltext.text(info);


    // position the text
    float lineLen = pdd->_drawAttrOverlay->_font.getLayoutLineLen();
    pdd->_drawAttrOverlay->_font.setLayoutLineLen(9999);

    PDrawAttr attrprev = pdd->_drawAttr;
    pdd->_drawAttr = pdd->_drawAttrOverlay;
    Point2d pt = _seltext.getTextSize(pdd);
    pdd->_drawAttr = attrprev;

    // TODO: adjust if it goes off the screen
    float marginX = 5, marginY = 5;
    Extents ext;
    ext.l = _selmx + marginX;
    ext.r = ext.l + pt.dX;
    ext.b = _selmy + marginY;
    ext.t = ext.b + pt.dY;
    _seltext.extents(ext);


    pdd->dataSel()->setSel(this);
}

//============================================================================
// todo: to properly validate, need to render the texture to a quad that matches
// the same pixel size on the screen as the pixels size of the texture.
//============================================================================
float GeoImgRaster::drawValidate(DrawData *pdd, FILE *pstFP)
{
    const char *func = "GeoImgRaster::drawValidate -";

    if (!_tx) return -1;
    if (!_tx->img()) return -1;
    if (!_dataValues) return -1;

    Extentsi extscr;
    vec3d tl, br;

    if (!UtlGL::objToScreen(_ext.l, _ext.t, 0, &tl))
    {
        LogError("%s failed to find top left screen coordinates, may need to zoom to extents", func);
        return -1;
    }

    if (!UtlGL::objToScreen(_ext.r, _ext.b, 0, &br))
    {
        LogError("%s failed to find bottom right screen coordinates, may need to zoom to extents", func);
        return -1;
    }

    extscr.l = (int)tl.x;
    extscr.r = (int)br.x;
    extscr.t = (int)tl.y;
    extscr.b = (int)br.y;


    MemBuf imgscr(4, extscr.width(), extscr.height());
    glReadPixels(extscr.l, extscr.b, extscr.width(), extscr.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgscr.getBuf());

    // compare each pixel on screen with the texture, and compute what we should have
    double u, v;
    double datav, datano, datana;
    std::vector<BYTE> cimg(4), cgrd(4), cscr(4);
    
    int scrloc = 0;
    BYTE *pscr = (BYTE *)imgscr.getBuf();
    bool loggedUnExpeced = false;
    int valid = 0;
    int totalcount = 0;
    int errorcount = 0;
    for (int y = extscr.b; y < extscr.t; y++)
    {
        // image is flipped so bottom is v == 1, top is v == 0
        v = 1 - ((double)(y - extscr.b) / (double)extscr.height());
        v = UtlMath::clamp(v, 0.0, 1.0);
        
        for (int x = extscr.l; x < extscr.r; x++)
        {
            totalcount++;

            u = (double)(x - extscr.l) / (double)extscr.width();
            u = UtlMath::clamp(u, 0.0, 1.0);

            cscr[0] = pscr[scrloc]; scrloc++;
            cscr[1] = pscr[scrloc]; scrloc++;
            cscr[2] = pscr[scrloc]; scrloc++;
            cscr[3] = pscr[scrloc]; scrloc++;

            int res = dataValidate(u, v, &datav, &datano, &datana, &cimg, &cgrd);
            if (res == -1)
            {
                valid++;
                continue; // no data
            }
            if (res == 0)
            {
                if (!loggedUnExpeced)
                {
                    loggedUnExpeced = true;
                    LogError("%s unexpected error validating at x: %d, y: %d", func, x, y);
                    if (pstFP)
                    {
                        fprintf(pstFP, "%s unexpected error validating at x: %d, y: %d", func, x, y);
                    }
                }
                continue;
            }
            
            

            // lets compare colors
            bool output = false;
            std::string msg;
            if (cimg != cscr)
            {
                msg = "img color not equal to screen color";
                output = true;
            }
            else if (cimg != cgrd)
            {
                msg = "img color not equal to gradient color";
                output = true;
            }

            if (!output)
            {
                valid++;
                continue;
            }

            errorcount++;

            std::string sout1, sout2;
            UtlString::format(sout1, "invalid x: %d, y: %d - %s\n", x, y, msg.c_str());
            UtlString::format(sout2, "invalid x: %d, y: %d\n", x, y);
            createInfoBlock(datav, datano, datana, cimg, cscr, cgrd, &sout2);
            
            sout1 += sout2;
            if (errorcount < 20)
            {
                LogError(sout1.c_str());
            }

            if (pstFP)
            {
                fprintf(pstFP, "%s\n\n", sout1.c_str());
            }
        }
    }

    // lets save the texture and screen images
    MemBuf cpy;
    cpy.copy(imgscr, true, true);
    std::string imgpath = pdd->filePaths()->m_pathBin + getName() + "_screen.png";
    if (!UtlQt::saveImg(&cpy, imgpath.c_str()))
    {
        LogError("%s failed to save screen image to: %s", func, imgpath.c_str());
    }

    cpy.copy(*_tx->img(), false, true);
    imgpath = pdd->filePaths()->m_pathBin + getName() + "_texture.png";
    if (!UtlQt::saveImg(&cpy, imgpath.c_str()))
    {
        LogError("%s failed to save texture image to: %s", func, imgpath.c_str());
    }

    float validpercent = (float)valid / (float)totalcount;
    return validpercent;
}

//============================================================================
//============================================================================
void GeoImgRaster::createInfoBlock(double datav, double datano, double datana, const std::vector<BYTE> &cimg, const std::vector<BYTE> &cscr, const std::vector<BYTE> &cgrd, std::string *info)
{
    // create info block text
    std::string strimgc, strscrc, strgrdc, strstat, strdata;
    UtlString::format(strimgc, "ImgC: %d,%d,%d,%d", cimg[0], cimg[1], cimg[2], cimg[3]);
    UtlString::format(strscrc, "ScrC: %d,%d,%d,%d", cscr[0], cscr[1], cscr[2], cscr[3]);
    UtlString::format(strgrdc, "GrdC: %d,%d,%d,%d", cgrd[0], cgrd[1], cgrd[2], cgrd[3]);

    if (_stats.getForceMin() == Stats::FORCE_MINMAX_OFF && _stats.getForceMax() == Stats::FORCE_MINMAX_OFF)
    {
        UtlString::format(strstat, "amin: %.2f, amax: %.2f, mean: %.2f\nomin:%.2f, omax:%.2f\nsdevs: %.2f",
            _stats.getMin(), _stats.getMax(), _stats.getMean(),
            _stats.getMinOrig(), _stats.getMaxOrig(),
            _stats.getForceSdevs());
    }
    else
    {
        UtlString::format(strstat, "amin: %.2f, amax: %.2f, mean: %.2f\nomin:%.2f, omax:%.2f\nsdevs: %.2f, fmin: %.2f, fmax: %.2f",
            _stats.getMin(), _stats.getMax(), _stats.getMean(),
            _stats.getMinOrig(), _stats.getMaxOrig(),
            _stats.getForceSdevs(), _stats.getForceMin(), _stats.getForceMax());
    }
    UtlString::format(strdata, "Data Value: %.2f, NormO: %.2f, NormA: %.2f", datav, datano, datana);

    UtlString::format(*info, "%s\n%s\n%s\n%s\n%s", strdata.c_str(), strstat.c_str(), strimgc.c_str(), strscrc.c_str(), strgrdc.c_str());
}

//============================================================================
// return - 0, unexpected error, -1 no data, 1 data
//============================================================================
int GeoImgRaster::dataValidate(float u, float v, double *datav, double *datano, double *datana, vector<BYTE> *cimg, vector<BYTE> *cgrd, vector<BYTE> *cscr, float sx, float sy)
{
    if (!_tx) return 0;
    if (!_tx->img()) return 0;
    if (!_tx->img()->getSize()) return 0;

    // now we can find the pixel color value in the image
    int bytesperpix = _tx->img()->getSize();
    int w = _tx->img()->getLenX();
    int h = _tx->img()->getLenY();

    int py = v * (h - 1); // get to the correct row
    int px = u * (w - 1); // get to the correct col
    int loc = (py * bytesperpix * w) + (px * bytesperpix);

    if (loc + bytesperpix >= _tx->img()->getByteCount())
    {
        return 0; // unexpected, out of bounds
    }

    BYTE *buf = (BYTE *)_tx->img()->getBuf();
    *cimg = { 0, 0, 0, 255 };
    (*cimg)[0] = buf[loc + 0];
    if (bytesperpix >= 3)
    {
        (*cimg)[1] = buf[loc + 1];
        (*cimg)[2] = buf[loc + 2];
        if (bytesperpix == 4)
        {
            (*cimg)[3] = buf[loc + 3];
        }
    }

    // if zero alpha then no data
    if ((*cimg)[3] == 0)
    {
        return -1;
    }

    // find the data value
    double *pdata = (double*)_dataValues->getBuf();
    *datav = pdata[py * w + px];

    // compute normalized value
    *datana = _stats.getPosNorm(*datav);
    *datano = _stats.getPosNormOrig(*datav);

    // get the screen color
    if (cscr)
    {
        *cscr = { 0, 0, 0, 255 };
        glReadPixels(sx, sy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &(*cscr)[0]);
    }

    // get the gradient color
    *cgrd = { 0, 0, 0, 255 };
    QColor c = _picker->pickByPercent(*datana);
    (*cgrd)[0] = c.red();
    (*cgrd)[1] = c.green();
    (*cgrd)[2] = c.blue();
    (*cgrd)[3] = c.alpha();

    return 1;
}
