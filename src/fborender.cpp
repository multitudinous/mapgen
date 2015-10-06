#include "fborender.h"

//============================================================================
//============================================================================
FboRender::FboRender() :
    _msaaOn(true)
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
void FboRender::setDrawParams(const Fbo::SDrawParams &params)
{
    _drawParams = params;

    updateDrawParams();
}

//============================================================================
//============================================================================
void FboRender::updateDrawParams()
{
    if (_fboc) _fboc->setDrawParams(_drawParams);
    if (_fboa) _fboa->setDrawParams(_drawParams);
}

//============================================================================
//============================================================================
Fbo::SDrawParams* FboRender::getDrawParams()
{
    return &_drawParams;
}
//============================================================================
//============================================================================
bool FboRender::drawStart(bool msaaOn, bool saveStates, bool clearColor, bool clearDepth)
{
    _msaaOn = msaaOn;

    updateDrawParams();

    Fbo *f = getFboPrimary();
    if (!f) return false;

    return f->drawStart(saveStates, clearColor, clearDepth);
}

//============================================================================
//============================================================================
void FboRender::drawEnd()
{
    Fbo *fp = getFboPrimary();
    if (!fp) return;

    Fbo *fs = getFboSecondary();
    return fp->drawEnd(fs);
}

//============================================================================
//============================================================================
bool FboRender::create(PCamera camera, GLuint width, GLuint height, bool aaOn, GLsizei msamples)
{
    destroy();

    if (aaOn)
    {
        _fboa.reset(new Fbo());
        if (!_fboa->create(camera, width, height, true, msamples)) return false;
    }

    _fboc.reset(new Fbo());
    if (!_fboc->create(camera, width, height, false)) return false;

    updateDrawParams();

    return true;
}

/*
//============================================================================
//============================================================================
bool FboRender::create(PCamera camera, GLuint width, GLuint height, bool aaOn)
{
    destroy();

    if (aaOn)
    {
        _fboa.reset(new Fbo());
        if (!_fboa->create(camera, width, height, true)) return false;
    }

    _fboc.reset(new Fbo());
    if (!_fboc->create(camera, width, height, false)) return false;

    return true;
}
*/

//============================================================================
//============================================================================
void FboRender::destroy()
{
    _fboc.reset();
    _fboa.reset();
}

//============================================================================
//============================================================================
Camera* FboRender::camera()
{
    Fbo *fp = getFboPrimary();
    if (!fp) return NULL;

    return fp->camera();
}

//============================================================================
//============================================================================
Fbo* FboRender::getFboPrimary()
{
    if (_msaaOn && _fboa) return _fboa.get();
    return _fboc.get();
}

//============================================================================
//============================================================================
Fbo* FboRender::getFboSecondary()
{
    if (_msaaOn && _fboa) return _fboc.get();

    return NULL;
}