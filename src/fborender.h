#ifndef FBORENDER_H
#define FBORENDER_H

#include "sysdefs.h"
#include "fbo.h"

class GlObj;
class DrawData;

class FboRender
{
public: 
    enum RenderFlags
    {
        E_RF_NONE = 0,
        E_RF_SUPERSAMPLE = 1,
        E_RF_JITTER = 2,
        E_RF_MULTISAMPLE = 4,
        E_RF_OVERRIDE_NONE = 32,
    };

public:
    FboRender();
    virtual ~FboRender();

    void setCameraExtents2d(const Extents &ext, bool refreshOrtho);

    void setDrawParams(const Fbo::SDrawParams &params);
    Fbo::SDrawParams* getDrawParams();
    void updateDrawParams();

    void draw(GlObj *obj, DrawData *dd, int rfOverride = E_RF_OVERRIDE_NONE, bool drawToScreen = false);

    void blitToScreen(DrawData *dd);

    //bool drawStart(bool msaaOn=true, bool saveStates = true, bool clearColor = true, bool clearDepth = true);
    //void drawEnd();

    bool create(PCamera camera, GLuint width, GLuint height, uint32_t renderFlags = E_RF_NONE, GLsizei msaaSamps = 16, GLsizei ssaaMul = 2, GLsizei jtaaSamps = 16, double jtaaoffsetPixels=1);
    //bool create(PCamera camera, GLuint width, GLuint height, bool aaOn = true);
    void destroy();

    Fbo* getFboCb() { return _fbocb.get();  }
    //Camera* camera();

protected:
    //Fbo* getFboPrimary();
    //Fbo* getFboSecondary();

    void drawJitter(GlObj *obj, DrawData *dd);

protected:
    PFbo _fbocb; // normal color buffer
    PFbo _fbosa; // for super sample antialiasing
    PFbo _fboja; // for jitter antialiasing
    PFbo _fboma; // multisample antialiasing
    std::vector<PFbo> _fboList;
    uint32_t _renderFlags;
    GLsizei _ssaaMul;
    GLsizei _jtaaSamps;
    double _jtaaOfsPixels;

    Fbo::SDrawParams _drawParams;
    PTexture _tx;
};

typedef shared_ptr<FboRender> PFboRender;
#endif