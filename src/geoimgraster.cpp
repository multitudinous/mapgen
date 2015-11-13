#include "geoimgraster.h"
#include "utlmath.h"
#include "utlgl.h"
#include "geotext.h"
#include "utlstring.h"

//============================================================================
//============================================================================
GeoImgRaster::GeoImgRaster() :
_cselimg({0,0,0,0}),
_cselscr({ 0,0,0,0}),
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

    // get the screen color
    _cselscr = { 0, 0, 0, 255 };
    glReadPixels(pdd->dataSel()->mx(), pdd->dataSel()->my(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &_cselscr[0]);

    // find the data value
    double *pdata = (double*)_dataValues->getBuf();
    _seldatav = pdata[py * w + px];

    // compute normalized value
    _seldatana = _stats.getPosNorm(_seldatav);
    _seldatano = _stats.getPosNormOrig(_seldatav);


    _selmx = pdd->dataSel()->mx();
    _selmy = pdd->dataSel()->my();

    // create info block text
    std::string strimgc, strscrc, strstat, strdata, str;
    UtlString::format(strimgc, "ImgC: %d,%d,%d,%d", _cselimg[0], _cselimg[1], _cselimg[2], _cselimg[3]);
    UtlString::format(strscrc, "ScrC: %d,%d,%d,%d", _cselscr[0], _cselscr[1], _cselscr[2], _cselscr[3]);
    UtlString::format(strstat, "amin: %.2f, amax: %.2f, mean: %.2f\nomin:%.2f, omax:%.2f\nforce - sdevs: %.2f, fmin: %.2f, fmax: %.2f", 
                        _stats.getMin(), _stats.getMax(), _stats.getMean(), 
                        _stats.getMinOrig(), _stats.getMaxOrig(), 
                        _stats.getForceSdevs(), _stats.getForceMin(), _stats.getForceMax());
    UtlString::format(strdata, "Data Value: %.2f, NormO: %.2f, NormA: %.2f", _seldatav, _seldatano, _seldatana);

    UtlString::format(str, "%s\n%s\n%s\n%s", strdata.c_str(), strstat.c_str(), strimgc.c_str(), strscrc.c_str());

    _seltext.text(str);


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
