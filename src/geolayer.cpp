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