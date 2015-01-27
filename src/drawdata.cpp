#include "drawdata.h"

DrawData::DrawData() :
    _cfg(new Config())
{
	m_shadersOn = true;
	m_wireframe = false;
	m_lightingOn = true;

    //m_frustum.reset(new Frustum());

	m_hGluTess = gluNewTess();
}
