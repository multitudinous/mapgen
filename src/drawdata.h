#ifndef DRAWDATA_H
#define DRAWDATA_H

#include "config.h"
#include "drawattr.h"
#include "shaderprog.h"
#include "movesys.h"
#include "datasel.h"
#include "datamouse.h"
#include "datavalidate.h"
#include "filepaths.h"

class DrawData
{
public:
	DrawData();

    bool init();

    void filePaths(PFilePaths fp);
    FilePaths* filePaths();

    DataSel* dataSel();
    DataMouse* dataMouse();
    DataValidate* dataValidate();

protected:
    bool initOverlay();

public:

    int _winW;
    int _winH;

    PConfig _cfg;
    PDrawAttr _drawAttr;
    PDrawAttr _drawAttrOverlay;
    PShaderProg _progSmoothLine;

    PFilePaths _filePaths;
    PMoveSys _movesys;
    PDataSel _ds;
    PDataMouse _dm;
    PDataValidate _dv;

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
