#include "fbo.h"
#include "utlgl.h"

#ifdef GDIPLUS
    #include "utlgdiplus.h"
#endif
#ifdef QT
    #include <QImage>
#endif

//============================================================================
//============================================================================
Fbo::Fbo()
{
    m_fboid = 0;
    m_cboid = 0;
    m_dboid = 0;
    m_txid = 0;
    m_w = 0;
    m_h = 0;
    _aaOn = false;
    _samples = 16;

    m_drawParams.colr[0] = 0;
    m_drawParams.colr[1] = 0;
    m_drawParams.colr[2] = 0;
    m_drawParams.colr[3] = 1;
    m_drawParams.znr = 10;
    m_drawParams.zfr = 10000;
}

//============================================================================
//============================================================================
Fbo::~Fbo()
{
    destroy();
}

//============================================================================
//============================================================================
bool Fbo::create(PCamera camera, GLuint width, GLuint height, bool aaOn, GLsizei msamples)
{
    // create the texture
    if ( !m_tx || (m_tx->GetWidth() != width) || (m_tx->GetHeight() != height) )
    {
        m_tx.reset(new Texture());
        bool txresult = false;

        if (aaOn)
        {
            txresult = m_tx->CreateMultisamp(width, height, msamples);
        }
        else
        {
            txresult = m_tx->Create(width, height, Texture::I_FILTER_NONE);
        }

        if (!txresult) return false;
            
    }

    return create(camera, width, height, m_tx->GetId(), aaOn, msamples);
}

//============================================================================
//============================================================================
bool Fbo::create(PCamera camera, GLuint width, GLuint height, GLuint txid, bool aaOn, GLsizei msamples)
{
    UtlGL::logErrorCheck("Fbo::create - error at the start");

    destroy(false);

    _camera = camera;

    m_w = width;
    m_h = height;
    m_txid = txid;
    if ( m_tx && (m_tx->GetId() != txid) ) m_tx.reset();

    _aaOn = aaOn;
    _samples = msamples;

    //GLint samples = 0;
    //glGetIntegerv(GL_MAX_SAMPLES, &samples);
    bool success = true;

    // create a framebuffer object, you need to delete them when program exits.
    glGenFramebuffers(1, &m_fboid);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboid);

    // color render buffer
    glGenRenderbuffers(1, &m_cboid);
    glBindRenderbuffer(GL_RENDERBUFFER, m_cboid);
    if (aaOn)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples, GL_RGBA8, width, height);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_cboid);

    UtlGL::logErrorCheck("Fbo::create - error after color render buffer");

    // depth render buffer
    glGenRenderbuffers(1, &m_dboid);
    glBindRenderbuffer(GL_RENDERBUFFER, m_dboid);
    if (aaOn)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples, GL_DEPTH24_STENCIL8, width, height);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_dboid);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_dboid);

    UtlGL::logErrorCheck("Fbo::create - error after depth render buffer");
    

    // bind the texture
    if (aaOn)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, txid, 0);
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, txid, 0);
    }

    UtlGL::logErrorCheck("Fbo::create - error after frame buffer texture");

    // check FBO status
    logFboInfo();
    bool status = checkStatus();
    if(!status)
    {
        success = false;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!success) destroy();

    UtlGL::logErrorCheck("Fbo::create - error at the end");

    return success;
}

//============================================================================
//============================================================================
bool Fbo::drawStart(bool saveStates, bool clearColor, bool clearDepth)
{
    UtlGL::logErrorCheck("Fbo::drawStart - error at the start");

    if (m_fboid <= 0 || m_w <= 0 || m_h <= 0)
    {
        m_drawPrev.drawSuccess = false;
        return false;
    }

    m_drawPrev.saveStates = saveStates;
    if (saveStates)
    {
        glGetIntegerv(GL_VIEWPORT, m_drawPrev.vp);
        //glGetFloatv(GL_PROJECTION_MATRIX, m_drawPrev.matPers);
        //glGetFloatv(GL_MODELVIEW_MATRIX, m_drawPrev.matModl);
        glGetFloatv(GL_COLOR_CLEAR_VALUE, m_drawPrev.colrClear);

        m_drawPrev.aaOn = glIsEnabled(GL_MULTISAMPLE);

        // save camera state
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
    }

    if (_camera)
    {
        _camera->refresh();
    }

    /*
    glViewport(0, 0, m_w, m_h);

    glMatrixMode(GL_PROJECTION);
    if (saveStates) glPushMatrix();
    glLoadIdentity();
    gluPerspective(m_drawParams.fov, (GLfloat)(m_w)/(GLfloat)(m_h), m_drawParams.znr, m_drawParams.zfr);

    glMatrixMode(GL_MODELVIEW);
    if (saveStates) glPushMatrix();
    glLoadIdentity();
    */

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboid);

    if (_aaOn)
    {
        glEnable(GL_MULTISAMPLE);
    }

    // clear buffer
    GLbitfield msk = 0;
    if (clearColor) msk |= GL_COLOR_BUFFER_BIT;
    if (clearDepth) msk |= GL_DEPTH_BUFFER_BIT;
    glClearColor(m_drawParams.colr[0], m_drawParams.colr[1], m_drawParams.colr[2], m_drawParams.colr[3]);
    glEnable(GL_DEPTH_TEST);
    if (msk) glClear(msk);

    // TODO: MAKE GENERIC
    // START OF GIS
    glDisable(GL_DEPTH_TEST);

    // for smooth lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // also hits for smooth lines and polys
    glEnable( GL_LINE_SMOOTH );
    //glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    glAlphaFunc(GL_GREATER, 0);
    glEnable(GL_ALPHA_TEST);
    // END GIS

    m_drawPrev.drawSuccess = true;

    UtlGL::logErrorCheck("Fbo::drawStart - error at the end");
    return true;
}

//============================================================================
// fboResult - if this is not null, the result rendering to this fbo will be blit copied to the result fbo,
//              this is useful for if the render fbo is using multisampling antialiasing and you need to get that into a usuable texture for screenshot or some other post processing.
//
//============================================================================
void Fbo::drawEnd(Fbo *fboResult)
{
    UtlGL::logErrorCheck("Fbo::drawEnd - error at start");

    if (!m_drawPrev.drawSuccess) return;

    if (m_drawParams.genMipMaps)
    {
        glBindTexture(GL_TEXTURE_2D, m_txid);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glFlush();
    glFinish();

    UtlGL::logErrorCheck("Fbo::drawEnd - error after glFinish");


    if (fboResult)
    {
        // Now resolve multisampled buffer(s) into intermediate FBO
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboid);
        //glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboResult->getFboId());
        //glDrawBuffer(GL_COLOR_ATTACHMENT0);
        //glBlitFramebuffer(0, 0, m_w, m_h, 0, 0, m_w, m_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBlitFramebuffer(0, 0, m_w, m_h, 0, 0, m_w, m_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        UtlGL::logErrorCheck("Fbo::drawEnd - failed to blit the framebuffer");
    }

    if (m_drawParams.saveFrame)
    {
        if (_aaOn && fboResult)
        {
            fboResult->saveFrame(m_drawParams.saveFrameBuf, m_drawParams.saveFrameStride, m_drawParams.saveFrameFlip);
        }
        else
        {
            saveFrame(m_drawParams.saveFrameBuf, m_drawParams.saveFrameStride, m_drawParams.saveFrameFlip);
        }

        UtlGL::logErrorCheck("Fbo::drawEnd - error after save frame");
    }

    if (m_drawParams.saveScreenShot)
    {
        if (_aaOn && fboResult)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, fboResult->getFboId());
            fboResult->saveFrame(m_drawParams.pathScreenShot.c_str());
        }
        else
        {
            saveFrame(m_drawParams.pathScreenShot.c_str());
        }

        UtlGL::logErrorCheck("Fbo::drawEnd - error after save screen shot");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!m_drawPrev.saveStates) return;

    glViewport(m_drawPrev.vp[0], m_drawPrev.vp[1], m_drawPrev.vp[2], m_drawPrev.vp[3]);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glClearColor(m_drawPrev.colrClear[0], m_drawPrev.colrClear[1], m_drawPrev.colrClear[2], m_drawPrev.colrClear[3]);

    if (m_drawPrev.aaOn)
    {
        glEnable(GL_MULTISAMPLE);
    }
    else
    {
        glDisable(GL_MULTISAMPLE);
    }

    m_drawPrev.drawSuccess = false;
}

//============================================================================
//============================================================================
bool Fbo::saveFrame(BYTE *pBuf, int stride, bool flip)
{
    if (!pBuf) return false;

    // must go line by line because of padding
    GLenum format = GL_BGRA; //GL_BGR; //GL_RGBA;
    //GLenum format = GL_RGBA; //GL_BGR;
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    BYTE *prow = pBuf;
    if (stride == 0) stride = m_w * 4;

    if (flip)
    {
        for (GLint i=(GLint)m_h-1; i>=0; i--)
        {
            glReadPixels(0, i, m_w, 1, format, GL_UNSIGNED_BYTE, prow );
            prow += stride;
        }
    }
    else
    {
        for (GLuint i=0; i<m_h; i++)
        {
            glReadPixels(0, i, m_w, 1, format, GL_UNSIGNED_BYTE, prow );
            prow += stride;
        }
    }

    return true;
}

//============================================================================
//============================================================================
bool Fbo::saveFrame(const char *path, bool flip)
{
    const char *func = "Fbo::SaveFrame() - ";
#ifdef GDIPLUS

    shared_ptr<Gdiplus::Bitmap> img = UtlGdiPlus::CreateImage(m_w, m_h, false);
    if (!img) return false;

    Gdiplus::BitmapData bd;
    if (!UtlGdiPlus::LockBits(img.get(), &bd))
    {
        return false;
    }

    SaveFrame((BYTE *)bd.Scan0, bd.Stride, flip);

    UtlGdiPlus::UnlockBits(img.get(), &bd);

    if (!UtlGdiPlus::SavePng(img.get(), path))
    {
        return false;
    }

    return true;

#elif defined (QT)

    QImage img(m_w, m_h, QImage::Format_ARGB32);
    saveFrame((BYTE *)img.bits(), img.bytesPerLine(), flip);
    if (!img.save(path))
    {
        LogError("%s - failed to save frame to: %s", func, path);
        return false;
    }

    return true;

#else
    return false;
#endif


}

//============================================================================
//============================================================================
void Fbo::destroy(bool destroyTx)
{
    if (m_fboid > 0) glDeleteFramebuffers(1, &m_fboid);
    if (m_cboid > 0) glDeleteRenderbuffers(1, &m_cboid);
    if (m_dboid > 0) glDeleteRenderbuffers(1, &m_dboid);

    m_fboid = 0;
    m_cboid = 0;
    m_dboid = 0;
    m_txid = 0;
    m_w = 0;
    m_h = 0;

    if (destroyTx) m_tx.reset();
}

//============================================================================
//============================================================================
bool Fbo::checkStatus()
{
    const char *func = "Fbo::CheckStatus() -";

    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        LogError("%s Framebuffer incomplete: Attachment is NOT complete.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        LogError("%s Framebuffer incomplete: No image is attached to FBO.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        LogError("%s Framebuffer incomplete: Attached images have different dimensions.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        LogError("%s Framebuffer incomplete: Color attached images have different internal formats.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        LogError("%s Framebuffer incomplete: Draw buffer.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        LogError("%s Framebuffer incomplete: Read buffer.", func);
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        LogError("%s Unsupported by FBO implementation.", func);
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        LogError("%s Multisample sample count not the same for all buffers.", func);
        return false;
    default:
        LogError("%s Unknow error.", func);
        return false;
    }
}

//============================================================================
//============================================================================
void Fbo::logFboInfo()
{
    const char *func = "Fbo::LogFboInfo() -";

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    LogTrace("%s Max Number of Color Buffer Attachment Points: %d", func, colorBufferCount);

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for(int i = 0; i < colorBufferCount; ++i)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                 GL_COLOR_ATTACHMENT0+i,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                                 &objectType);
        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                     GL_COLOR_ATTACHMENT0+i,
                                                     GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                     &objectId);

            std::string formatName;

            if(objectType == GL_TEXTURE)
            {
                LogTrace("%s Color Attachment %d: GL_TEXTURE: %s", func, i, UtlGL::GetTextureInfo(objectId).c_str());
            }
            else if(objectType == GL_RENDERBUFFER)
            {
                LogTrace("%s Color Attachment %d: GL_RENDERBUFFER: %s", func, i, UtlGL::GetRenderbufferInfo(objectId).c_str());
            }
            else
            {
                LogTrace("%s Color Attachment %d: UNKNOWN", func, i);
            }
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                             GL_DEPTH_ATTACHMENT,
                                             GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                             &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                 GL_DEPTH_ATTACHMENT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                 &objectId);

        switch(objectType)
        {
        case GL_TEXTURE:
            LogTrace("%s Depth Attachment: GL_TEXTURE: %s", func, UtlGL::GetTextureInfo(objectId).c_str());
            break;
        case GL_RENDERBUFFER:
            LogTrace("%s Depth Attachment: GL_RENDERBUFFER: %s", func, UtlGL::GetRenderbufferInfo(objectId).c_str());
            break;
        default:
            LogTrace("%s Depth Attachment: UNKNOWN", func);
        }
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                             GL_STENCIL_ATTACHMENT,
                                             GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                             &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                 GL_STENCIL_ATTACHMENT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                 &objectId);
        switch(objectType)
        {
        case GL_TEXTURE:
            LogTrace("%s Stencil Attachment: GL_TEXTURE: %s", func, UtlGL::GetTextureInfo(objectId).c_str());
            break;
        case GL_RENDERBUFFER:
            LogTrace("%s Stencil Attachment: GL_RENDERBUFFER: %s", func, UtlGL::GetRenderbufferInfo(objectId).c_str());
            break;
        default:
            LogTrace("%s Stencil Attachment: UNKNOWN", func);
        }
    }
}
