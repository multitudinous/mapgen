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
    m_rboid = 0;
    m_txid = 0;
    m_w = 0;
    m_h = 0;

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
bool Fbo::create(PCamera camera, GLuint width, GLuint height)
{
    if (m_tx == NULL || m_tx->GetWidth() != width || m_tx->GetHeight() != height)
    {
        m_tx.reset(new Texture());
        if (!m_tx->Create(width, height, Texture::I_FILTER_NONE))
        {
            return false;
        }
    }

    return create(camera, width, height, m_tx->GetId());
}

//============================================================================
//============================================================================
bool Fbo::create(PCamera camera, GLuint width, GLuint height, GLuint txid)
{
    destroy(false);

    _camera = camera;

    m_w = width;
    m_h = height;
    m_txid = txid;
    if (m_tx != NULL && m_tx->GetId() != txid) m_tx.reset();

    //GLint samples = 0;
    //glGetIntegerv(GL_MAX_SAMPLES_EXT, &samples);
    bool success = true;
    // create a framebuffer object, you need to delete them when program exits.
    glGenFramebuffersEXT(1, &m_fboid);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboid);

    glGenRenderbuffersEXT(1, &m_rboid);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rboid);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8, width, height); // depth and stencil
    // glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height); // depth buffer only
    //glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GL_DEPTH_COMPONENT, width, height);
    //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, txid, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT, m_rboid); // depth and stenicl
    //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_rboid); // depth buffer only


    // check FBO status
    logFboInfo();
    bool status = checkStatus();
    if(!status)
    {
        success = false;
    }

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    if (!success) destroy();

    return success;
}

//============================================================================
//============================================================================
bool Fbo::drawStart(bool saveStates, bool clearColor, bool clearDepth)
{
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

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboid);

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
    return true;
}

//============================================================================
//============================================================================
void Fbo::drawEnd()
{
    if (!m_drawPrev.drawSuccess) return;

    if (m_drawParams.genMipMaps)
    {
        glBindTexture(GL_TEXTURE_2D, m_txid);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (m_drawParams.saveFrame)
    {
        glFlush();
        glFinish();
        saveFrame(m_drawParams.saveFrameBuf, m_drawParams.saveFrameStride, m_drawParams.saveFrameFlip);
    }

    if (m_drawParams.saveScreenShot)
    {
        saveFrame(m_drawParams.pathScreenShot.c_str());
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    if (!m_drawPrev.saveStates) return;

    glViewport(m_drawPrev.vp[0], m_drawPrev.vp[1], m_drawPrev.vp[2], m_drawPrev.vp[3]);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glClearColor(m_drawPrev.colrClear[0], m_drawPrev.colrClear[1], m_drawPrev.colrClear[2], m_drawPrev.colrClear[3]);

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
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
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
    if (m_fboid > 0) glDeleteFramebuffersEXT(1, &m_fboid);
    if (m_rboid > 0) glDeleteRenderbuffersEXT(1, &m_rboid);

    m_fboid = 0;
    m_rboid = 0;
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
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        LogError("%s Framebuffer incomplete: Attachment is NOT complete.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        LogError("%s Framebuffer incomplete: No image is attached to FBO.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        LogError("%s Framebuffer incomplete: Attached images have different dimensions.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        LogError("%s Framebuffer incomplete: Color attached images have different internal formats.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        LogError("%s Framebuffer incomplete: Draw buffer.", func);
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        LogError("%s Framebuffer incomplete: Read buffer.", func);
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
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
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &colorBufferCount);
    LogTrace("%s Max Number of Color Buffer Attachment Points: %d", func, colorBufferCount);

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for(int i = 0; i < colorBufferCount; ++i)
    {
        glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
                                                 GL_COLOR_ATTACHMENT0_EXT+i,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT,
                                                 &objectType);
        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
                                                     GL_COLOR_ATTACHMENT0_EXT+i,
                                                     GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
                                                     &objectId);

            std::string formatName;

            if(objectType == GL_TEXTURE)
            {
                LogTrace("%s Color Attachment %d: GL_TEXTURE: %s", func, i, UtlGL::GetTextureInfo(objectId).c_str());
            }
            else if(objectType == GL_RENDERBUFFER_EXT)
            {
                LogTrace("%s Color Attachment %d: GL_RENDERBUFFER_EXT: %s", func, i, UtlGL::GetRenderbufferInfo(objectId).c_str());
            }
            else
            {
                LogTrace("%s Color Attachment %d: UNKNOWN", func, i);
            }
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
                                             GL_DEPTH_ATTACHMENT_EXT,
                                             GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT,
                                             &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
                                                 GL_DEPTH_ATTACHMENT_EXT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
                                                 &objectId);

        switch(objectType)
        {
        case GL_TEXTURE:
            LogTrace("%s Depth Attachment: GL_TEXTURE: %s", func, UtlGL::GetTextureInfo(objectId).c_str());
            break;
        case GL_RENDERBUFFER_EXT:
            LogTrace("%s Depth Attachment: GL_RENDERBUFFER_EXT: %s", func, UtlGL::GetRenderbufferInfo(objectId).c_str());
            break;
        default:
            LogTrace("%s Depth Attachment: UNKNOWN", func);
        }
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
                                             GL_STENCIL_ATTACHMENT_EXT,
                                             GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT,
                                             &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
                                                 GL_STENCIL_ATTACHMENT_EXT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
                                                 &objectId);
        switch(objectType)
        {
        case GL_TEXTURE:
            LogTrace("%s Stencil Attachment: GL_TEXTURE: %s", func, UtlGL::GetTextureInfo(objectId).c_str());
            break;
        case GL_RENDERBUFFER_EXT:
            LogTrace("%s Stencil Attachment: GL_RENDERBUFFER_EXT: %s", func, UtlGL::GetRenderbufferInfo(objectId).c_str());
            break;
        default:
            LogTrace("%s Stencil Attachment: UNKNOWN", func);
        }
    }
}
