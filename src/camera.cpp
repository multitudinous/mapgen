#include "camera.h"
#include "sysdefs.h"

//============================================================================
//============================================================================
Camera::Camera(ECameraMode mode) :
    _mode(mode),
    _pos(0,0,0),
    _rot(0,0,0),
    _zoom(1),
    _pan(0,0)
{
    //_mode = mode;
    //_pos.SetVector(0,0,0);
    //_rot.SetVector(0,0,0);
    //_zoom = 1;
}

//============================================================================
//============================================================================
Camera::Camera(const Camera &c) :
_mode(c._mode),
_winRect(c._winRect),
_zoom(c._zoom),
_pos(c._pos),
_rot(c._rot),
_pan(c._pan),
_extOrig(c._extOrig),
_ext(c._ext),
_extMod(c._extMod),
_zmin(c._zmin),
_zmax(c._zmax)
{

}

//============================================================================
//============================================================================
void Camera::initOrtho(int winW, int winH, int left, int bottom, bool setViewport, bool updateMats)
{
    Extents ext(left, bottom + winH, left + winW, bottom);
    _extOrig = ext;
    onResize(winW, winH, left, bottom, setViewport, updateMats);
}

//============================================================================
//============================================================================
double Camera::pixelSizeToWorld(double pixelCount)
{
    double worldAmount = pixelCount * _extMod.width() / _winRect.width();
    return worldAmount;
}

//============================================================================
//============================================================================
void Camera::zoomSet(double zoom)
{
    _zoom = zoom;
    setProjection();
}

//============================================================================
//============================================================================
void Camera::zoomInc(double amt)
{
    _zoom += amt;
    setProjection();
}

//============================================================================
//============================================================================
void Camera::panSet(const Point2d &pan)
{
    _pan = pan;
    setProjection();
}

//============================================================================
//============================================================================
void Camera::panInc(double x, double y)
{
    _pan.dX += x;
    _pan.dY += y;
    setProjection();
}

//============================================================================
//============================================================================
void Camera::panByPixel(double dxp, double dyp)
{
    double dxw = pixelSizeToWorld(dxp);
    double dyw = pixelSizeToWorld(dyp);
    panInc(dxw, dyw);
}

//============================================================================
//============================================================================
void Camera::setMode(ECameraMode mode)
{
     _mode = mode;
     setProjection();
}

//============================================================================
//============================================================================
void Camera::setExtents2d(const Extents &ext, bool refreshOrtho)
{
    _extOrig = ext;
    if (refreshOrtho)
    {
         _mode = Ortho;
        setProjection();
    }
}


//============================================================================
//============================================================================
void Camera::setView(bool clearPrev)
{
    GLfloat xtrans, ytrans, ztrans;
    GLfloat lookupdown, scenerot, lean;

    // update positions
    lookupdown = _rot.x;
    lean       = _rot.z;
    scenerot   = 360.0f - _rot.y;
    xtrans = -_pos.x;
    ytrans = -_pos.y;
    ztrans = -_pos.z;

    // render correct view
    glMatrixMode(GL_MODELVIEW);
    if (clearPrev) glLoadIdentity();

    glRotatef(lookupdown, 1.0f, 0.0f, 0.0f);
    glRotatef(lean, 0,0,1);
    glRotatef(scenerot, 0.0f, 1.0f, 0.0f);
    glTranslatef(xtrans, ytrans, ztrans);
}

//============================================================================
//============================================================================
void Camera::refresh()
{
    glViewport((GLsizei)_winRect.l, (GLsizei)_winRect.b, (GLsizei)_winRect.width(), (GLsizei)_winRect.height());
    setProjection();
}

//============================================================================
//============================================================================
void Camera::onResize(int winx, int winy, int left, int bottom, bool setViewport, bool updateMats)
{
    if (setViewport)
    {
        glViewport(left, bottom, winx, winy);
    }

    _winRect.l = left;
    _winRect.b = bottom;
    _winRect.r = left + winx;
    _winRect.t = bottom + winy;

    setProjection(updateMats);
}

//============================================================================
//============================================================================
void Camera::setProjection(bool updateMats)
{
    if (_mode == Ortho)
    {
        setOrthoZoom(_extOrig, updateMats);
    }
    else if (updateMats)
    {
        setPerspective(_winRect.width(), _winRect.height());
    }
}


//============================================================================
//============================================================================
void Camera::setPerspective(double winW, double winH)
{
    if (winH <= 0) winH = 1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, winW / winH, 0.1, 10000.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}


//============================================================================
//============================================================================
void Camera::computeExtentFromWidth(const Extents &extOrig, double winW, double winH, Extents *extNew, bool adjust)
{
	GLdouble dx = extOrig.width();
	GLdouble dy = extOrig.height();

	extNew->l = extOrig.l;
	extNew->r = extOrig.r;

	GLdouble dY = dx * winH / winW;

	// lets center
	GLdouble yCenter = extOrig.cy();
	extNew->b = yCenter - dY / 2.0;
	extNew->t = yCenter + dY / 2.0;

	if (adjust)
	{
		if (extNew->height() < extOrig.height())
		{
			computeExtentFromHeight(extOrig, winW, winH, extNew, false);
		}
	}
}

//============================================================================
//============================================================================
void Camera::computeExtentFromHeight(const Extents &extOrig, double winW, double winH, Extents *extNew, bool adjust)
{
	GLdouble dx = extOrig.width();
	GLdouble dy = extOrig.height();

	extNew->t = extOrig.t;
	extNew->b = extOrig.b;

	GLdouble dX = dy * winW / winH;

	// lets center
	GLdouble xCenter = extOrig.cx();
	extNew->l = xCenter - dX / 2.0;
	extNew->r = xCenter + dX / 2.0;

	if (adjust)
	{
		if (extNew->width() < extOrig.width())
		{
			computeExtentFromWidth(extOrig, winW, winH, extNew, false);
		}
	}
}

//============================================================================
//============================================================================
void Camera::setOrthoZoom(const Extents &ext, bool updateMats)
{
	// OK, now save the dimensions of the window
    double width = _winRect.width();
    double height = _winRect.height();

    if (width <= 0) width = 1;
    if (height <= 0) height = 1;

    _extOrig = ext;
	_zmax = 100;
	_zmin = -_zmax;

	GLdouble dx = ext.width();
	GLdouble dy = ext.height();

	if(fabs(dx) > 0.001 || fabs(dy) > 0.001)//Controls how far you can zoom in
	{
		if (dx >= dy)
		{
			computeExtentFromWidth(ext, width, height, &_ext, true);
		}
		else
		{
			computeExtentFromHeight(ext, width, height, &_ext, true);
		}
		/*
		if(dx >= dy)
        {
            _ext.l = ext.l;
            _ext.r = ext.r;

            GLdouble dY = dx * height / width;

             // lets center
            GLdouble yCenter = ext.cy();
            _ext.b = yCenter - dY/2.0;
            _ext.t = yCenter + dY/2.0;
		}
		else
		{
            _ext.t = ext.t;
            _ext.b = ext.b;

            GLdouble dX = dy * width / height;

            // lets center
            GLdouble xCenter = ext.cx();
            _ext.l = xCenter - dX/2.0;
            _ext.r = xCenter + dX/2.0;
	
		}
		*/

		/*
		double scale = std::fmin(width/dx, height/dy);
		GLdouble xcenter = ext.cx();
		GLdouble ycenter = ext.cy();
		GLdouble neww = dx * scale;
		GLdouble newh = dy * scale;
		_ext.setFromCenter(xcenter, ycenter, neww, newh);
		*/

	}
    else
    {
        _ext = ext;
        _ext.expand(1);
    }

    _extMod = _ext;
    _extMod.zoom(_zoom);
    _extMod.move(_pan);

    //LogTrace("ortho center %.2f, %.2f", _ext.cx(), _ext.cy());
    //LogTrace("zoom  center %.2f, %.2f", extZ.cx(), extZ.cy());

    if (updateMats)
    {
        setOrthoMats();
    }
}

//============================================================================
//============================================================================
void Camera::setOrthoMats()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
        //glOrtho( _ext.l, _ext.r, _ext.b, _ext.t, _zmin, _zmax);
    glOrtho(_extMod.l, _extMod.r, _extMod.b, _extMod.t, _zmin, _zmax);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

