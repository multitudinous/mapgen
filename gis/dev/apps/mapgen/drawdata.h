#ifndef DRAWDATA_H
#define DRAWDATA_H

#include "sysdefs.h"

//#include "light.h"
//#include "frustum.h"

#include "config.h"
#include "drawattr.h"

class DrawData
{
public:
	DrawData();

public:

    PConfig _cfg;
    PDrawAttr _drawAttr;

	bool m_shadersOn;
	bool m_wireframe;
	bool m_lightingOn;

    //std::vector<PLight> m_lights;

    //PFrustum m_frustum;

	GLUtesselator *m_hGluTess;


};

typedef shared_ptr<DrawData> PDrawData;

#endif
