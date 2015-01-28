#ifndef FBO_H
#define FBO_H

#include "sysdefs.h"
#include "camera.h"
#include "texture.h"

class Fbo
{
protected:
    struct SDrawPrev
    {
        //GLfloat matPers[16];
        //GLfloat matModl[16];
        GLfloat colrClear[4];
        GLint vp[4];
        bool saveStates;
        bool drawSuccess;
    };

    struct SDrawParams
    {
        GLfloat fov;
        GLfloat znr;
        GLfloat zfr;
        GLfloat colr[4];
        bool genMipMaps;
        bool saveScreenShot;
        string pathScreenShot;

        bool saveFrame;
        BYTE *saveFrameBuf;
        int saveFrameStride;
        bool saveFrameFlip;

        SDrawParams(GLfloat fieldOfView=60.0, GLfloat nearPlane=1.0f, GLfloat farPlane=100000.0f, float cr=0, float cg=0, float cb=0, float ca=1)
        {
            fov = fieldOfView;
            znr = nearPlane;
            zfr = farPlane;
            colr[0] = cr;
            colr[1] = cg;
            colr[2] = cb;
            colr[3] = ca;
            genMipMaps = false;
            saveScreenShot = false;
            saveFrame = false;
            saveFrameBuf = NULL;
            saveFrameStride = 0;
            saveFrameFlip = false;
        }
    };

public:
    Fbo();
    virtual ~Fbo();

    void setDrawParams(SDrawParams params) { m_drawParams = params; }
    SDrawParams* getDrawParams() { return &m_drawParams; }
    bool drawStart(bool saveStates=true, bool clearColor=true, bool clearDepth=true);
    void drawEnd();

    bool create(PCamera camera, GLuint width, GLuint height);
    bool create(PCamera camera, GLuint width, GLuint height, GLuint txid);
    void destroy(bool destroyTx=true);

    GLuint getTxId() { return m_txid; }

    Camera* camera() { return _camera.get(); }

protected:

    bool saveFrame(BYTE *pBuf, int stride, bool flip=true);
    bool saveFrame(const char *path, bool flip=true);

    bool checkStatus();
    void logFboInfo();

protected:
    GLuint m_fboid;
    GLuint m_rboid;
    GLuint m_txid;
    GLuint m_w;
    GLuint m_h;
    PTexture m_tx;

    SDrawParams m_drawParams;
    SDrawPrev m_drawPrev;

    PCamera _camera;
};

typedef shared_ptr<Fbo> PFbo;

#endif
