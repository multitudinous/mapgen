#ifndef DRAWDATA_H
#define DRAWDATA_H

#include "config.h"
#include "drawattr.h"
#include "shaderprog.h"
#include "movesys.h"
#include "datasel.h"
#include "datamouse.h"

class DrawData
{
public:
	DrawData();

    bool init();

    DataSel* dataSel();
    DataMouse* dataMouse();

protected:
    bool initOverlay();

public:

    int _winW;
    int _winH;

    PConfig _cfg;
    PDrawAttr _drawAttr;
    PDrawAttr _drawAttrOverlay;
    PShaderProg _progSmoothLine;

    PMoveSys _movesys;
    PDataSel _ds;
    PDataMouse _dm;

	bool m_shadersOn;
	bool m_wireframe;
	bool m_lightingOn;

    bool _drawSelected;

    //std::vector<PLight> m_lights;

    //PFrustum m_frustum;

	GLUtesselator *m_hGluTess;


};

typedef shared_ptr<DrawData> PDrawData;

#endif
