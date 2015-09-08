#ifndef FBORENDER_H
#define FBORENDER_H

#include "sysdefs.h"
#include "fbo.h"

class FboRender
{
public:
    FboRender();
    virtual ~FboRender();

    void setDrawParams(const Fbo::SDrawParams &params);
    Fbo::SDrawParams* getDrawParams();
    void updateDrawParams();

    bool drawStart(bool msaaOn=true, bool saveStates = true, bool clearColor = true, bool clearDepth = true);
    void drawEnd();

    bool create(PCamera camera, GLuint width, GLuint height, GLsizei msamples);
    //bool create(PCamera camera, GLuint width, GLuint height, bool aaOn = true);
    void destroy();

    Camera* camera();

protected:
    Fbo* getFboPrimary();
    Fbo* getFboSecondary();

protected:
    PFbo _fboc; // normal color buffer
    PFbo _fboa; // multisample antialiasing
    bool _msaaOn;

    Fbo::SDrawParams _drawParams;
};

typedef shared_ptr<FboRender> PFboRender;
#endif