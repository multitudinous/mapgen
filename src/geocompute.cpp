#include "geocompute.h"
#include "geowords.h"
#include "geopoly.h"
#include "utlgeom2d.h"
#include "utlqt.h"
#include <QFile>
#include <QTextStream>

//============================================================================
//============================================================================
GeoCompute::GeoCompute() :
    _verbose(false)
{

}

//============================================================================
//============================================================================
void drawBox(const box2d &box, float *c)
{
    float cprev[4];
    if (c)
    {
        glGetFloatv(GL_CURRENT_COLOR, cprev);
        glColor4fv(c);
    }

    glBegin(GL_LINE_LOOP);
        glVertex2d(box.l, box.b);
        glVertex2d(box.r, box.b);
        glVertex2d(box.r, box.t);
        glVertex2d(box.l, box.t);
    glEnd();

    if (c)
    {
        glColor4fv(cprev);
    }
}

//============================================================================
//============================================================================
void GeoCompute::draw(DrawData *pdd)
{
    PDrawAttr attr;
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        // find a polygon
        GlObj *obj = _childList[i].get();
        for (unsigned int i=0; i<obj->childList().size(); i++)
        {
            // find a polygon
            GlObj *child = obj->childList()[i].get();
            GeoPoly *poly = dynamic_cast<GeoPoly*>(child);
            if (poly)
            {
                poly->draw(pdd);
                if (!attr.get()) attr = poly->getDrawAttr();
            }
        }
    }

    GlText *pfont = DrawAttr::font(pdd->_drawAttr, attr);
    if (!pfont) return;

    bool drawWord = false;
    bool drawBoundingBox = true;

    Rgbf colorBoxH(0,1,0,1);
    Rgbf colorBoxV(0,0,1,1);
    Rgbf color = DrawAttr::colorLabels(pdd->_drawAttr, attr);

    Rgbf debugColor[6];
    debugColor[0] = Rgbf(1,0,0,1);
    debugColor[1] = Rgbf(0,1,0,1);
    debugColor[2] = Rgbf(0,0,1,1);
    debugColor[3] = Rgbf(1,1,0,1);
    debugColor[4] = Rgbf(0,1,1,1);
    debugColor[5] = Rgbf(1,0,1,1);
    int debugCur = 0;

    // draw the words
    WordMap::iterator it = _wordMap.begin();
    while (it != _wordMap.end())
    {
        WordsList *pwords = it->second.get();
        std::string sec = it->first;
        it++;

        bool debug = false;
        /*
        if (!sec.compare("36.00"))
        {
            debug = true;
        }
        */

        for (unsigned int i=0; i<pwords->size(); i++)
        {
            float *afcolor = color.m_af;
            if (debug)
            {
                afcolor = debugColor[debugCur].m_af;
                debugCur++;
                if (debugCur > 5) debugCur = 0;
            }

            const GeoWords::WordPos *wp = &(*pwords)[i];
            Point2d c;
            wp->box.getCenter(&c.dX, &c.dY);

            if (drawWord)
            {
                glColor4fv(afcolor);
                pfont->renderC(wp->word.c_str(), c);
            }

            if (drawBoundingBox)
            {
                if (debug)
                {
                    drawBox(wp->box, afcolor);
                }
                else
                {
                    if (wp->orient == 'h')
                        drawBox(wp->box, colorBoxH.m_af);
                    else
                        drawBox(wp->box, colorBoxV.m_af);
                }
            }

        }
    }
}

//============================================================================
//============================================================================
void GeoCompute::computeBox(DrawData *pdd, box3d *pbox)
{
    // only compute box from polys and word map.
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        // find a polygon
        GlObj *child = _childList[i].get();
        GeoPoly *poly = dynamic_cast<GeoPoly*>(child);
        if (poly)
        {
            poly->computeBox(pdd, pbox);
        }
    }

    WordMap::iterator it = _wordMap.begin();
    while (it != _wordMap.end())
    {
        WordsList *pwords = it->second.get();
        it++;

        for (unsigned int i=0; i<pwords->size(); i++)
        {
            const GeoWords::WordPos *wp = &(*pwords)[i];
            pbox->UpdateBox(wp->box.l, wp->box.t, 0);
            pbox->UpdateBox(wp->box.r, wp->box.b, 0);

        }
    }
}

//============================================================================
//============================================================================
bool GeoCompute::compute()
{
    if (_type == "wordfinder")
    {
        return computeWordFinder();
    }

    LogError("GeoCompute::compute() - Unrecognized compute type: %s", _type.c_str());
    return false;
}

//============================================================================
//============================================================================
bool GeoCompute::computeWordFinder()
{
    const char *func = "GeoCompute::computeWordFinder() -";

    // find the root objects we need
    GeoWords *pWords = NULL;
    GlObj *pPolys = NULL;

    for (unsigned int i=0; i<_childList.size(); i++)
    {
        if (pWords && pPolys)
        {
            break;
        }

        GlObj *obj =_childList[i].get();

        if (!pWords)
        {
            pWords = dynamic_cast<GeoWords*>(obj);
            if (pWords)
            {
                continue;
            }
        }

        if (!pPolys)
        {
            for (unsigned int i=0; i<obj->childList().size(); i++)
            {
                // find a polygon
                GlObj *child = obj->childList()[i].get();
                GeoPoly *poly = dynamic_cast<GeoPoly*>(child);
                if (poly)
                {
                    pPolys = obj;
                    break;
                }
            }

            if (pPolys)
            {
                continue;
            }
        }
    }

    if (!pWords || !pPolys)
    {
        LogError("%s failed to find words or polys objects to compute from", func);
        return false;
    }

    if (_name == "t95n_r46w")
    {
        int idebug = 1;
    }

    box3d boxWords, boxPolys;
    pWords->computeBox(NULL, &boxWords);
    pPolys->computeBox(NULL, &boxPolys);

    // TODO: SHOULD WE MAKE A COPY OF THE DATA AND TRANSFORM IT, LEAVING ORIGINAL INTACT

    // scale the polys to match words box
    vec3d szWords = boxWords.GetSize();
    vec3d szPolys = boxPolys.GetSize();

    double scaleX = szWords.x / szPolys.x;
    double scaleY = scaleX; // don't have a height to scale with so use x scale, which it should be the same anyway.
    if (pWords->getComputeBoxMode() == GeoWords::BoxCompute_TLBR)
    {
        scaleY = szWords.y / szPolys.y;
    }

    // scale
    pPolys->scale(scaleX, scaleY, 0);

    // recompute box
    boxPolys.Reset();
    pPolys->computeBox(NULL, &boxPolys);

    // update the height now if we need to and get the updated word bounding box
    if (pWords->updateBoxHeightIfNeeded(boxPolys.GetSize().y))
    {
        boxWords.Reset();
        pWords->computeBox(NULL, &boxWords);
    }
    /*
    if (pWords->isComputeBoxAuto())
    {
        // need to recompute word box height based on polys bounding box height
        boxWords.vmin.y = boxWords.vmax.y - boxPolys.GetSize().y;
    }
    */

    vec3d fr = boxPolys.GetCenter();
    vec3d to = boxWords.GetCenter();
    pPolys->moveFromTo(fr, to);

    // now that polygons have been transformed into word space, lets find what words fit into which poly
    _wordMap.clear();
    //WordMap wordMap;
    WordsList wordsNoLoc;
    WordsList words = pWords->getWordsJoined();
    WordsList wordsSec = pWords->getWordsSections();
    PolyMap polyMap;
    SecValidation secVal;

    getFeaturePolymap(pPolys, polyMap);
    validateSections(wordsSec, polyMap, &secVal);
    locateWords(words, polyMap, _wordMap, wordsNoLoc);
    outputWords(_wordMap, wordsNoLoc, polyMap, secVal);

    return true;
}

//============================================================================
//============================================================================
void GeoCompute::getFeaturePolymap(GlObj *parent, PolyMap &map)
{
    // fill our polymap
    for (unsigned int i=0; i<parent->childList().size(); i++)
    {
        GlObj *obj = parent->childList()[i].get();
        GeoPoly *poly = dynamic_cast<GeoPoly*>(obj);
        if (!poly) continue;

        std::string fvalue;
        poly->getFeatureValue(_feature, &fvalue);
        if (fvalue.size() <= 0)
        {
            char ac[16];
            sprintf(ac, "%d", i);
            fvalue = ac;
        }

        map[fvalue] = poly;
    }
}

//============================================================================
//============================================================================
void GeoCompute::validateSections(const WordsList &words, const PolyMap &polys, SecValidation *secVal)
{
    *secVal = SecValidation();
    secVal->secCount =(int)words.size();
    if (words.size() <= 0)
    {
        LogTrace("No word section numbers found to validate from");
        return;
    }

    for (unsigned int i=0; i<words.size(); i++)
    {
        const GeoWords::WordPos *wp = &words[i];

        PolyMap::const_iterator it;
        if (!locateWord(wp, polys, &it))
        {
            continue;
        }

        // sections are floats, but should just be ints
        std::string sec = it->first;
        int polySec = (int)atof(sec.c_str());

        int wordSec = atoi(wp->word.c_str());

        if (polySec == wordSec)
        {
            secVal->secFound++;
        }
    }

    // return percent of sections found
    secVal->perFound = (float)secVal->secFound / (float)secVal->secCount;

    LogTrace("Section Validation - count: %d, located: %d, percent: %.2f", secVal->secCount, secVal->secFound, secVal->perFound);
}

//============================================================================
//============================================================================
void GeoCompute::locateWords(const WordsList &words, const PolyMap &polys, WordMap &wordMap, WordsList &wordsNoLoc)
{
    for (unsigned int i=0; i<words.size(); i++)
    {
        const GeoWords::WordPos *wp = &words[i];

        PolyMap::const_iterator it;
        if (!locateWord(wp, polys, &it))
        {
            wordsNoLoc.push_back(*wp);
            continue;
        }

        std::string feature = it->first;
        PWordsList pwords;
        WordMap::iterator iter = wordMap.find(feature);
        if (iter != wordMap.end() )
        {
            pwords = iter->second;
        }
        else
        {
            pwords.reset(new WordsList);
            wordMap[feature] = pwords;
        }

        pwords->push_back(*wp);

        /*
        Point2d c;
        wp->box.getCenter(&c.dX, &c.dY);

        bool found = false;
        PolyMap::const_iterator it = polys.begin();
        while (it != polys.end())
        {
            GeoPoly *poly = it->second;          
            if (!UtlGeom2d::ptInPoly(c, &poly->getPts()[0], (int)poly->size()))
            {
                it++;
                continue;
            }

            std::string feature = it->first;
            PWordsList pwords;
            WordMap::iterator iter = wordMap.find(feature);
            if (iter != wordMap.end() )
            {
                pwords = iter->second;
            }
            else
            {
                pwords.reset(new WordsList);
                wordMap[feature] = pwords;
            }

            pwords->push_back(*wp);
            found = true;
            break;
        }

        if (!found)
        {
            wordsNoLoc.push_back(*wp);
        }
        */
    }
}

//============================================================================
//============================================================================
bool GeoCompute::locateWord(const GeoWords::WordPos *wp, const PolyMap &polys, PolyMap::const_iterator *pit)
{
    Point2d c;
    wp->box.getCenter(&c.dX, &c.dY);

    PolyMap::const_iterator it = polys.begin();
    while (it != polys.end())
    {
        GeoPoly *poly = it->second;
        if (!UtlGeom2d::ptInPoly(c, &poly->getPts()[0], (int)poly->size()))
        {
            it++;
            continue;
        }

        if (pit) *pit = it;
        return true;
    }

    return false;
}

//============================================================================
//============================================================================
void GeoCompute::getPolyVerbose(const PolyMap &polyMap, const std::string &feature, std::string *pOut)
{
    PolyMap::const_iterator it = polyMap.find(feature);
    if (it == polyMap.end()) return;

    box3d box;
    GeoPoly *poly = it->second;
    poly->computeBoxNoLabel(&box);

    vec3d c = box.GetCenter();

    double l = box.vmin.x;
    double t = box.vmax.y;
    double r = box.vmax.x;
    double b = box.vmin.y;
    double w = box.GetSize().x;
    double h = box.GetSize().y;

    char frmt[512];
    sprintf(frmt, "l,t,r,b: %.2f, %.2f, %.2f, %.2f, center(%.2f, %.2f), w: %.2f, h: %.2f", l, t, r, b, c.x, c.y, w, h);
    *pOut = frmt;
}

//============================================================================
//============================================================================
void GeoCompute::getWordVerbose(const GeoWords::WordPos &wp, std::string *pOut)
{
    double cx, cy;
    wp.box.getCenter(&cx, &cy);

    double w = wp.box.width();
    double h = wp.box.height();

    char frmt[512];
    sprintf(frmt, "l,t,r,b: %.2f, %.2f, %.2f, %.2f, center(%.2f, %.2f), w: %.2f, h: %.2f, fontsz: %.2f", wp.box.l, wp.box.t, wp.box.r, wp.box.b, cx, cy, w, h, wp.fontSz);
    *pOut = frmt;
}

//============================================================================
//============================================================================
void GeoCompute::outputValidation(const SecValidation &secVal)
{
    const char *func = "GeoCompute::outputValidation() -";

    if (_validationFile.size() <= 0) return;

    QString filename(_validationFile.c_str());
    QFile file(filename);
    if (!file.open(QIODevice::Append))
    {
        LogError("%s Failed to open validation file for append: %s", func, _validationFile.c_str());
        return;
    }

    QTextStream stream(&file);
    outputValidation(stream, secVal);

    file.close();
}

//============================================================================
//============================================================================
void GeoCompute::outputValidation(QTextStream &stream, const SecValidation &secVal)
{
    QString nl = UtlQt::newline();

    stream << "Section Validation - " << QString(_name.c_str()) << nl;
    stream << "-------------------------------------" << nl;
    stream << "section count: " << secVal.secCount  << nl;
    stream << "sections located: " << secVal.secFound  << nl;
    stream << "valid percent: " << secVal.perFound  << nl;
    stream << nl;
    stream << nl;
}

//============================================================================
//============================================================================
void GeoCompute::outputWords(const WordMap &wordMap, const WordsList &wordsNoLoc, const PolyMap &polyMap, const SecValidation &secVal)
{
    const char *func = "GeoCompute::outputWords() -";

    outputValidation(secVal); // output validation to its own file

    QString filename(_output.c_str());
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        LogError("%s Failed to open output file: %s", func, _output.c_str());
        return;
    }

    QTextStream stream(&file);
    QString nl = UtlQt::newline();

    outputValidation(stream, secVal); // output validation to this file
    /*
    stream << "Section Validation" << nl;
    stream << "-------------------------------------" << nl;
    stream << "section count: " << secVal.secCount  << nl;
    stream << "sections located: " << secVal.secFound  << nl;
    stream << "valid percent: " << secVal.perFound  << nl;
    stream << nl;
    stream << nl;
    */

    WordMap::const_iterator it = wordMap.begin();
    while (it != wordMap.end())
    {
        std::string feature = it->first;
        WordsList *pwords = it->second.get();
        it++;

        stream << _feature.c_str() << " " << feature.c_str() << nl;
        if (_verbose)
        {
            std::string out;
            getPolyVerbose(polyMap, feature, &out);
            stream << out.c_str() << nl;
        }
        stream << "-------------------------------------" << nl;

        for (unsigned int i=0; i<pwords->size(); i++)
        {
            const GeoWords::WordPos *wp = &(*pwords)[i];
            stream << wp->word.c_str() << " ";
            if (_verbose)
            {
                std::string out;
                getWordVerbose(*wp, &out);
                stream << out.c_str();
            }
            stream << nl;
        }

        stream << nl;
    }

    stream << "Words Not Located" << nl;
    stream << "*****************************************" << nl;
    for (unsigned int i=0; i<wordsNoLoc.size(); i++)
    {
        const GeoWords::WordPos *wp = &wordsNoLoc[i];
        stream << wp->word.c_str() << " ";
        if (_verbose)
        {
            std::string out;
            getWordVerbose(*wp, &out);
            stream << out.c_str();
        }
        stream << nl;
    }

    file.close();
}
