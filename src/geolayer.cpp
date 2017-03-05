#include "geolayer.h"
#include <iostream>
#include <fstream>
#include "drawattr.h"

//============================================================================
//============================================================================
GeoLayer::GeoLayer() :
    _renderFlags(0)
{
}

#include "geopoly.h"
#include "gldraw.h"
#include "box2.h"
#include "utlglline.h"

void drawLineLoop(const ListPt2d &pts, float w, float r, float g, float b)
{
	const Point2d *pt1 = NULL, *pt2 = NULL;
	for (int i = 0; i<pts.size(); i++)
	{
		pt2 = &pts.at(i);
		if (pt1 == NULL)
		{
			pt1 = pt2;
			continue;
		}

		Gldraw::drawLine(pt1->dX, pt1->dY, pt2->dX, pt2->dY, w, r, g, b);
		pt1 = pt2;

		break;
	}

	if (pts.size() <= 2) return;

	pt2 = &pts.at(0); // first point
	//Gldraw::drawLine(pt1->dX, pt1->dY, pt2->dX, pt2->dY, w, r, g, b); // draw last point to first point line

}

void drawLineShader(DrawData *pdd, const ListPt2d &pts, float linew, float r, float g, float b)
{
	PVbo _vboOutline = UtlGlLine::createLined(pts, linew, UtlGlLine::round_join, UtlGlLine::square_cap);

	if (!_vboOutline) return;

	pdd->_progSmoothLine->enable();
	GLuint uidColr = pdd->_progSmoothLine->getUniformLoc("ucolor");
	if (uidColr != -1)
	{
		float c4[4] { r, g, b, 1};
		glUniform4fv(uidColr, 1, c4);
	}

	_vboOutline->draw();
	pdd->_progSmoothLine->disable();
}

//============================================================================
//============================================================================
bool GeoLayer::drawMaskOutInner(DrawData *pdd)
{
	if (!_draw) return false;

	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;

	if (!DrawAttr::drawPolyFill(pdd->_drawAttr, _drawAttr))
	{
		pdd->_drawAttr = drawprev;
		return false;
	}

	std::vector< const ListPt2d*> innerList;

	int count = 0;
	for (unsigned int i = 0; i < _childList.size(); i++)
	{
		for (unsigned int c = 0; c < _childList[i]->childList().size(); c++)
		{
			GeoPoly *poly = dynamic_cast<GeoPoly *>(_childList[i]->childList()[c].get());
			if (!poly) continue;
			if (!poly->getInnerRings().size()) continue;

			for (size_t i = 0; i < poly->getInnerRings().size(); i++)
			{
				innerList.push_back(&poly->getInnerRings()[i]->getPts());
				count++;
			}
		}
	}

	if (!count)
	{
		return false;
	}

	bool maskOutPrev = pdd->_maskOutInner;
	pdd->_maskOutInner = true;

	// set up the mask
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	for (size_t i = 0; i < innerList.size(); i++)
	{
		if (innerList[i]->size() < 3) continue;
		Gldraw::drawPolyConcaveVN(pdd->m_hGluTess, *innerList[i]);
	}

	glStencilFunc(GL_NOTEQUAL, 1, 1);    // Draw where inner polys are not
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	GlObj::draw(pdd);

	glDisable(GL_STENCIL_TEST);

	pdd->_maskOutInner = maskOutPrev;
	pdd->_drawAttr = drawprev;

	return true;
}

//============================================================================
//============================================================================
void GeoLayer::drawDebug(DrawData *pdd)
{

	if (GeoLayer::drawMaskOutInner(pdd)) return;

	/*
	if (_name == "field_fill")
	{
	if (!_draw) return;

	int count = 0;
	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;
	for (unsigned int i = 0; i < _childList.size(); i++)
	{
	for (unsigned int c = 0; c < _childList[i]->childList().size(); c++)
	{
	GeoPoly *poly = dynamic_cast<GeoPoly *>(_childList[i]->childList()[c].get());
	if (!poly) continue;
	if (!poly->getInnerRings().size()) continue;

	poly->draw(pdd);

	}
	}

	pdd->_drawAttr = drawprev;
	return;
	}

	if (_name == "field_outline")
	{
	if (!_draw) return;

	int count = 0;
	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;
	for (unsigned int i = 0; i < _childList.size(); i++)
	{
	for (unsigned int c = 0; c < _childList[i]->childList().size(); c++)
	{
	GeoPoly *poly = dynamic_cast<GeoPoly *>(_childList[i]->childList()[c].get());
	if (!poly) continue;
	if (!poly->getInnerRings().size()) continue;

	Gldraw::drawLineLoopBasic(poly->getPts(), 4, 1, 1, 0);
	for (unsigned int h = 0; h < poly->getInnerRings().size(); h++)
	{

	Gldraw::drawLineLoopBasic(poly->getInnerRings()[i]->getPts(), 3, 0, 0, 1);
	//Gldraw::drawLineLoop(poly->getPts(), 1, 1, 1, 0);
	//drawLineLoop(poly->getPts(), 1, 1, 1, 0);
	//drawLineShader(pdd, poly->getPts(), 1, 1, 1, 0);
	//Gldraw::drawPts(poly->getPts(), 2, 1, 0, 0);
	//_childList[i]->childList()[c]->draw(pdd);
	count++;
	}

	}
	}

	pdd->_drawAttr = drawprev;
	}
	else
	{
	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;

	GlObj::draw(pdd);

	pdd->_drawAttr = drawprev;
	}

	if (_name == "field_outline")
	{
	if (!_draw) return;

	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;

	int drawItem = 3;
	for (unsigned int i = 0; i < _childList.size(); i++)
	{
	for (unsigned int c = 0; c < _childList[i]->childList().size(); c++)
	{

	if (c == drawItem)
	{
	GeoPoly *poly = dynamic_cast<GeoPoly *>(_childList[i]->childList()[c].get());
	if (poly)
	{
	box2d box;
	for (size_t i = 0; i < poly->getPts().size(); i++)
	{
	Point2d pt = poly->getPts()[i];
	box.update(pt.dX, pt.dY);
	}

	Gldraw::drawLine(Point2d(box.l, box.b), Point2d(box.r, box.b), 2, 0, 0, 1);
	Gldraw::drawLine(Point2d(box.r, box.b), Point2d(box.r, box.t), 2, 0, 0, 1);
	Gldraw::drawLine(Point2d(box.r, box.t), Point2d(box.l, box.t), 2, 0, 0, 1);
	Gldraw::drawLine(Point2d(box.l, box.t), Point2d(box.l, box.b), 2, 0, 0, 1);
	//Gldraw::drawLineLoopBasic(poly->getPts(), 1, 0, 1, 1);
	//Gldraw::drawLineLoop(poly->getPts(), .00001, 1, 1, 0);
	drawLineLoop(poly->getPts(), 2, 1, 1, 0);
	drawLineShader(pdd, poly->getPts(), 1, 1, 1, 0);
	Gldraw::drawPts(poly->getPts(), 2, 1, 0, 0);
	}
	//_childList[i]->childList()[c]->draw(pdd);
	}
	}
	}

	pdd->_drawAttr = drawprev;
	}

	else
	{
	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;

	GlObj::draw(pdd);

	pdd->_drawAttr = drawprev;
	}
	*/

	// TODO: restore original

	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;

	GlObj::draw(pdd);

	pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::draw(DrawData *pdd)
{
	PDrawAttr drawprev = pdd->_drawAttr;
	pdd->_drawAttr = _drawAttr;

	GlObj::draw(pdd);

	pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::computeBox(DrawData *pdd, box3d *pbox)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    GlObj::computeBox(pdd, pbox);

    pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::initAttr(DrawData *pdd)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    if (pdd->_drawAttr && _exportColorMapFile.size())
    {
        pdd->_drawAttr->_fillFeatureColorMap = true;

    }

    GlObj::initAttr(pdd);

    if (pdd->_drawAttr && _exportColorMapFile.size())
    {
        exportColorMap(pdd);
        pdd->_drawAttr->_fillFeatureColorMap = false;
        pdd->_drawAttr->_filledFeatureColorMap.clear();
    }

    pdd->_drawAttr = drawprev;
}

//============================================================================
//============================================================================
void GeoLayer::initLabels(DrawData *pdd)
{
    PDrawAttr drawprev = pdd->_drawAttr;
    pdd->_drawAttr = _drawAttr;

    GlObj::initLabels(pdd);

    pdd->_drawAttr = drawprev;
}


//============================================================================
//============================================================================
void GeoLayer::exportColorMap(DrawData *pdd)
{
    if (!pdd->_drawAttr) return;
    if (_exportColorMapFile.size() <= 0) return;

    std::ofstream file(_exportColorMapFile, std::ios::out);
    if (!file.is_open()) 
    { 
        LogError("GeoLayer::exportColorMap - failed to open json file for writing: %s", _exportColorMapFile.c_str());
        return;
    }

    file << "[\n";

    int count = 0;
    DrawAttr::FeatureColorMap::const_iterator it = pdd->_drawAttr->_filledFeatureColorMap.begin();
    while (it != pdd->_drawAttr->_filledFeatureColorMap.end())
    {
        if (count > 0) file << ",\n";
        count++;

        file << "{";
        file << "\"feature\": \"" << it->first << "\",";
        file << "\"color\": \"" << it->second << "\"";
        file << "}";

        it++;
    }

    file << "\n]";
    file.close();

    pdd->_drawAttr->_filledFeatureColorMap.clear();
}
