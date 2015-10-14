#include "fborender.h"
#include "GlObj.h"
#include "point2d.h"
#include "utlgeom2d.h"
#include "utlgl.h"
#include "glstates.h"
#ifdef QT
#include <QImage>
#endif

//============================================================================
//============================================================================
FboRender::FboRender() :
    _renderFlags(E_RF_NONE),
    _ssaaMul(2),
    _jtaaSamps(16)

{
    _drawParams.colr[0] = 0;
    _drawParams.colr[1] = 0;
    _drawParams.colr[2] = 0;
    _drawParams.colr[3] = 1;
    _drawParams.znr = 10;
    _drawParams.zfr = 10000;
}

//============================================================================
//============================================================================
FboRender::~FboRender()
{
    destroy();
}

//============================================================================
//============================================================================
bool FboRender::create(PCamera camera, GLuint width, GLuint height, uint32_t renderFlags, GLsizei msaaSamps, GLsizei ssaaMul, GLsizei jtaaSamps, double jtaaoffsetPixels)
{
    destroy();

    _renderFlags = renderFlags;

    bool needfboSS = true;
    int aw = width, ah = height;
    if (_renderFlags & E_RF_SUPERSAMPLE)
    {
        aw *= _ssaaMul;
        ah *= _ssaaMul;
    }

    if (_renderFlags & E_RF_MULTISAMPLE)
    {
        PCamera cam(new Camera(*camera));
        cam->onResize(aw, ah);

        _fboma.reset(new Fbo());

        if (!_fboma->create(cam, aw, ah, true, msaaSamps)) return false;
        _fboList.push_back(_fboma);
         
        // need super sample buffer because it seems you can't then blit this to a different sized fbo, but only the same size
    }

    if (_renderFlags & E_RF_JITTER)
    {
        PCamera cam(new Camera(*camera));
        cam->onResize(aw, ah);

        _fboja.reset(new Fbo());
        if (!_fboja->create(cam, aw, ah, false)) return false;
        _fboList.push_back(_fboja);
        needfboSS = false; // don't need super sample buffer because this is super sampled

        _jtaaSamps = jtaaSamps;
        _jtaaOfsPixels = jtaaoffsetPixels;
    }

    if (_renderFlags & E_RF_SUPERSAMPLE && needfboSS)
    {
        PCamera cam(new Camera(*camera));
        cam->onResize(aw, ah);

        _fbosa.reset(new Fbo());
        if (!_fbosa->create(cam, aw, ah, false)) return false;
        _fboList.push_back(_fbosa);
    }

    {
        PCamera cam(new Camera(*camera));

        _fbocb.reset(new Fbo());
        if (!_fbocb->create(cam, width, height, false)) return false;
        _fboList.push_back(_fbocb);
    }

    updateDrawParams();

    return true;
}


//============================================================================
//============================================================================
void FboRender::setCameraExtents2d(const Extents &ext, bool refreshOrtho)
{
    for (size_t i = 0; i < _fboList.size(); i++)
    {
        _fboList[i]->camera()->setExtents2d(ext, false);
    }
}

//============================================================================
//============================================================================
void FboRender::setDrawParams(const Fbo::SDrawParams &params)
{
    _drawParams = params;

    updateDrawParams();
}

//============================================================================
//============================================================================
void FboRender::updateDrawParams()
{
    for (size_t i = 0; i < _fboList.size(); i++)
    {
        _fboList[i]->setDrawParams(_drawParams);
    }
}

//============================================================================
//============================================================================
Fbo::SDrawParams* FboRender::getDrawParams()
{
    return &_drawParams;
}

//============================================================================
//============================================================================
void FboRender::draw(GlObj *obj, DrawData *dd, int rfOverride, bool drawToScreen)
{
    updateDrawParams();

    int rf = _renderFlags;
    if (rfOverride != E_RF_OVERRIDE_NONE)
    {
        rf = rfOverride;
    }

    if (rf & E_RF_JITTER)
    {
        drawJitter(obj, dd);
    }
    else if (rf & E_RF_MULTISAMPLE)
    {
        _fboma->drawStart(true, true, true);
        obj->draw(dd);

        if (rf & E_RF_SUPERSAMPLE)
        {
            _fboma->drawEnd(_fbosa.get());
            _fbosa->blit(_fbocb.get());
        }
        else
        {
            _fboma->drawEnd(_fbocb.get());
        }
        
    }
    else if (rf & E_RF_SUPERSAMPLE)
    {
        _fbosa->drawStart(true, true, true);
        obj->draw(dd);
        _fbosa->drawEnd(_fbocb.get());
    }
    else
    {
        _fbocb->drawStart(true, true, true);
        obj->draw(dd);
        _fbocb->drawEnd();
    }

    if (drawToScreen)
    {
        blitToScreen(dd);
    }
}

//============================================================================
//============================================================================
void FboRender::drawJitter(GlObj *obj, DrawData *dd)
{
    //ListPt2d offsets;

    // draw first frame with no offset
    float cbColrPrev[4];
    memcpy(cbColrPrev, _fbocb->getDrawParams()->colr, sizeof(float) * 4);
    _fbocb->getDrawParams()->colr[0] = 0;
    _fbocb->getDrawParams()->colr[1] = 0;
    _fbocb->getDrawParams()->colr[2] = 0;
    _fbocb->getDrawParams()->colr[3] = 0;

    _fbocb->drawStart(true, true, true);
    _fboja->drawStart(false, true, true);

    // camera is updated after drawStart called so should get a good result for pxdis here
    double pxdis = _fboja->camera()->pixelSizeToWorld(_jtaaOfsPixels);
    double radmov = Define::TWOPI / _jtaaSamps;
    double radcur = 0;

    /*
    obj->draw(dd);

    glFlush();
    glFinish();

    UtlGL::logErrorCheck("FboRender::drawJitter - error after glFinish with first render");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fboja->getFboId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbocb->getFboId());
    glBlitFramebuffer(0, 0, _fboja->getW(), _fboja->getH(), 0, 0, _fbocb->getW(), _fbocb->getH(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
    UtlGL::logErrorCheck("FboRender::drawJitter - failed to blit the framebuffer with first render");
    */
    
    
    // now render jitter frames with blending
    for (int i = 0; i < _jtaaSamps; i++)
    {
        Point2d pt;
        pt.dX = pxdis * cos(radcur);
        pt.dY = pxdis * sin(radcur);
        radcur += radmov;

        
        _fboja->drawStart(false, true, true); // don't save state cause we already did
        glTranslated(pt.dX, pt.dY, 0);
        obj->draw(dd);

        glFlush();
        glFinish();

        float colrBlendPrev[4];
        glGetFloatv(GL_BLEND_COLOR, colrBlendPrev);

        glBindFramebuffer(GL_FRAMEBUFFER, _fbocb->getFboId());
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE);
        glBlendColor(0, 0, 0, 1.0f / (float)_jtaaSamps);
        
        // refresh camera will setup correct viewport
        _fbocb->camera()->refresh();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // draw a full screen quad
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _fboja->getTxId());
        glGenerateMipmap(GL_TEXTURE_2D);

        /*
        // debug
#ifdef QT
        std::vector<GLubyte> imgPixels(_fboja->getW() * _fboja->getH() * 4);
        GLubyte *bytesSrc = &imgPixels[0];
        glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytesSrc);

        QImage img(_fboja->getW(), _fboja->getH(), QImage::Format_ARGB32);
        GLubyte *bytesDst = (GLubyte *)img.bits();

        for (int y = 0; y < _fboja->getH(); y++)
        {
            int ofsSrc = y * _fboja->getW() * 4;
            int ofsDst = y * img.bytesPerLine();
            memcpy(bytesDst + ofsDst, bytesSrc + ofsSrc, _fboja->getW() * 4);
        }

        std::string path = dd->_cfg->imgFolder() + "debug.png";
        if (!img.save(path.c_str()))
        {
            LogError("FboRender::drawJitter - failed to save frame to: %s", path.c_str());
            
        }
#endif
       */

        glColor4f(1, 1, 1, 1);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, 0);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, 0);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, 0);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, 0);
        glEnd();
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        glFlush();
        glFinish();

        glBlendColor(colrBlendPrev[0], colrBlendPrev[1], colrBlendPrev[2], colrBlendPrev[3]);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
    }

    // lets save if we need to
    if (_drawParams.saveScreenShot)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbocb->getFboId());
        _fbocb->saveFrame(_drawParams.pathScreenShot.c_str());

        UtlGL::logErrorCheck("FboRender::drawJitter - error after save screen shot");
    }

    memcpy(_fbocb->getDrawParams()->colr, cbColrPrev, sizeof(float) * 4);
    _fbocb->restoreStates();

}

//============================================================================
//============================================================================
void FboRender::blitToScreen(DrawData *dd)
{
    {
        Rect2d rcDstTot(0, dd->_winW, dd->_winH, 0);
        _fbocb->renderThis(rcDstTot, 0);
        return;
    }

    bool useW = true;

    Rect2d rcDstTot(0, dd->_winW, dd->_winH, 0);
    Rect2d rcDst;
    UtlGeom2d::fit(_fbocb->getW(), _fbocb->getH(), rcDstTot, &rcDst);



    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgramObjectARB(NULL);

    Rect2d bounds(-1, 1, 1, -1);
    rcDst.normalize(rcDstTot, bounds);

    GlStates states;
    states.push(GL_DEPTH_TEST, false);
    states.push(GL_MULTISAMPLE, false);
    states.push(GL_TEXTURE_2D, true);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // draw a full screen quad
    glBindTexture(GL_TEXTURE_2D, _fbocb->getTxId());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // create a texture from the fbo texture
    // TODO: figure out why just rendering with the fbo texture there is a gray overlay
    // shouldn't have to perform this copy ( needed mipmaps )

    /*
    PMemBuf memimg(new MemBuf(4, _fbocb->getW(), _fbocb->getH()));
    GLubyte *bytesSrc = (GLubyte *)memimg->getBuf();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytesSrc);

    if (!_tx || _tx->getWidth() != _fbocb->getW() || _tx->getHeight() != _fbocb->getH())
    {
        _tx.reset(new Texture());
        _tx->create(memimg, GL_CLAMP, Texture::I_FILTER_TRILINEAR);
        glBindTexture(GL_TEXTURE_2D, _tx->getId());
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, _tx->getId());
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _tx->getWidth(), _tx->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, memimg->getBuf());
    }
    */
    
    /*
#ifdef QT
    //GLubyte *bytesSrc = (GLubyte *)memimg->GetBuf();

    QImage img(_fbocb->getW(), _fbocb->getH(), QImage::Format_ARGB32);
    GLubyte *bytesDst = (GLubyte *)img.bits();

    for (int y = 0; y < _fbocb->getH(); y++)
    {
        int ofsSrc = y * _fbocb->getW() * 4;
        int ofsDst = y * img.bytesPerLine();
        memcpy(bytesDst + ofsDst, bytesSrc + ofsSrc, _fbocb->getW() * 4);
    }

    std::string path = dd->_cfg->imgFolder() + "debug_blit.png";
    if (!img.save(path.c_str()))
    {
        LogError("FboRender::blitToScreen - failed to save frame to: %s", path.c_str());

    }
#endif
    */

    

    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex3f(rcDst.l(), rcDst.b(), 1);
        glTexCoord2i(1, 0);
        glVertex3f(rcDst.r(), rcDst.b(), 1);
        glTexCoord2i(1, 1);
        glVertex3f(rcDst.r(), rcDst.t(), 1);
        glTexCoord2i(0, 1);
        glVertex3f(rcDst.l(), rcDst.t(), 1);
    glEnd();

    // restore
    glBindTexture(GL_TEXTURE_2D, 0);

     
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    states.popAll();

    UtlGL::logErrorCheck("FboRender::blitToScreen - failed to draw fbo to screen");
}

//============================================================================
//============================================================================
void FboRender::destroy()
{
    _fboList.clear();

    _fbocb.reset();
    _fbosa.reset();
    _fboja.reset();
    _fboma.reset();
}