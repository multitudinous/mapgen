#ifndef DRAWDATA_H
#define DRAWDATA_H

#include "config.h"
#include "drawattr.h"
#include "shaderprog.h"

class DrawData
{
public:
	DrawData();

    bool init();

public:

    int _winW;
    int _winH;

    PConfig _cfg;
    PDrawAttr _drawAttr;
    PShaderProg _progSmoothLine;

	bool m_shadersOn;
	bool m_wireframe;
	bool m_lightingOn;

    //std::vector<PLight> m_lights;

    //PFrustum m_frustum;

	GLUtesselator *m_hGluTess;


};

typedef shared_ptr<DrawData> PDrawData;

#endif
