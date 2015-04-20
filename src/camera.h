#ifndef CAMERA_H
#define CAMERA_H

#include "vector3.h"
#include "extents.h"
#include <memory>

class Camera
{
public:
    enum ECameraMode
    {
        Ortho,
        Perspective
    };

public:
    Camera(ECameraMode mode = Perspective);

    void refresh();
    void onResize(int winx, int winy, int left=0, int bottom=0);
    void setView(bool clearPrev=true);

    double pixelSizeToWorld(double pixelCount);

    double zoomGet() { return _zoom; }
    void zoomSet(double zoom);
    void zoomInc(double amt);

    Point2d panGet() { return _pan; }
    void panSet(const Point2d &pan);
    void panInc(double x, double y);
    void panByPixel(double dxp, double dyp);

    void setMode(ECameraMode mode);
    void setExtents2d(const Extents &ext, bool refreshOrtho=true);



protected:
    void setProjection();
    void setPerspective(double winW, double winH);
    void setOrthoZoom(const Extents &ext);

public:
     ECameraMode _mode;

     Extents _winRect;
     double _zoom;

	// for perspective 3d camera
    vec3f _pos;
    vec3f _rot; // in degrees

	// for ortho 2d
    Point2d _pan; // for  pan
    Extents _extOrig;
    Extents _ext; // modified to maintain aspect and fit window
    Extents _extMod; // extents after pan and zoom

	double _zmin;
	double _zmax;


};

typedef std::shared_ptr<Camera> PCamera;

#endif
