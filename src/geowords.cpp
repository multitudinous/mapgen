#include "geowords.h"
#include "utlstring.h"
#include <QFile>
#include <QTextStream>
#include "linkedlist.h"

//============================================================================
//============================================================================
GeoWords::GeoWords() :
    _removeNumeric(false),
    _minWordLen(1),
    _boxCompute(BoxCompute_Auto),
    _autoOfsL(0),
    _autoOfsR(0),
    _autoOfsTnB(-20),
    _sectionMin(1),
    _sectionMax(36),
    //_sectionFontSize(10.4016),
    _commaEscape("?$!")
{
    _sectionFontSizes.push_back(10.4016);
    _sectionFontSizes.push_back(9.3028);
    _removeList.push_back(std::string("ave"));
}


//============================================================================
//============================================================================
bool GeoWords::load(const char *file)
{
    const char *func = "GeoWords::load() -";

     _words.clear();
     _wordsValid.clear();
     _wordsJoined.clear();
     _wordsSections.clear();
     _box.reset();

    QFile inputFile(file);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        LogError("%s failed to open file %s", func, file);
        return false;
    }

    int linenum = 1;
    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        loadWord(line, linenum);
        linenum++;
    }
    inputFile.close();

    joinWords();

    computeWordBox();
    LogTrace("%s bounding box - l: %d, t: %d, r: %d, b: %d", func, _box.l, _box.t, _box.r, _box.b);

    return true;
}


//============================================================================
//============================================================================
bool GeoWords::loadWord(const QString &line, int linenum)
{
    const char *func = "GeoWords::loadWord() -";

    std::vector<std::string> results;
    UtlString::explode(line.toStdString(), ",", &results);

    if (results.size() < 5)
    {
        LogError("%s failed to parse line: %d, number of line items was: %d, should be 4", func, linenum, results.size());
        return false;
    }

    for (int i=0; i<5; i++)
    {
        if (results[i].size() <= 0)
        {
            LogError("%s failed to parse line: %d, empty string found for part number: %d", func, linenum, i);
            return false;
        }
    }

    WordPos wp;
    wp.word = UtlString::replaceStringCopy(results[0], _commaEscape, std::string(","));
    wp.box.l = atof(results[1].c_str());
    wp.box.t = atof(results[2].c_str());
    wp.box.r = atof(results[3].c_str());
    wp.box.b = atof(results[4].c_str());
    wp.fontSz = 1;
    if (results.size() > 5)
    {
        wp.fontSz = atof(results[5].c_str());
    }
    if (results.size() > 6 && results[6].size() > 0)
    {
        wp.orient = results[6].at(0);
        if (wp.orient != 'h' && wp.orient != 'v')
        {
            LogError("%s error on line: %d, unrecognized orient: %s", func, linenum, results[6].c_str());
            wp.orient = 'h';
        }
    }

    if (isDup(wp))
    {
        return false;
    }

    if (!isRemoveType(wp))
    {
        _wordsValid.push_back(wp);
    }

    if (isSection(wp))
    {
        _wordsSections.push_back(wp);
    }

    _words.push_back(wp);


    return true;
}

//============================================================================
//============================================================================
bool GeoWords::isRemoveType(const WordPos &wp)
{
    if (wp.word.size() < _minWordLen)
    {
        return true;
    }

    if (_removeNumeric)
    {
        if (UtlString::hasNumeric(wp.word))
        {
            return true;
        }
    }

    std::string cmp;
    UtlString::toLower(wp.word, &cmp);

    std::vector<std::string>::const_iterator it = find(_removeList.begin(), _removeList.end(), cmp);
    if (it != _removeList.end())
    {
        return true;
    }

    // remove 0 width or 0 height
    if (fabs(wp.box.width()) < .01)
    {
        return true;
    }
    if (fabs(wp.box.height()) < .01)
    {
        return true;
    }

    // remove 0 font
    if (fabs(wp.fontSz) < .01)
    {
        return true;
    }

    return false;
}

//============================================================================
// seems to be some duplicates in the list, lets remove them, they are usually 1 pixel apart
//============================================================================
bool GeoWords::isDup(const WordPos &wp)
{
    for (unsigned int i=0; i<_words.size(); i++)
    {
        if (_words[i].word  != wp.word)
        {
            continue;
        }

        double x1, y1, x2, y2;
        _words[i].box.getCenter(&x1, &y1);
        wp.box.getCenter(&x2, &y2);
        double space = abs(x1 - x2) + abs(y1 - y2);
        if (space <= 2.01)
        {
            return true;
        }
    }

    return false;
}

//============================================================================
//============================================================================
bool GeoWords::isSection(const WordPos &wp)
{
    if (!UtlString::isNumeric(wp.word))
    {
        return false;
    }

    int sec = atoi(wp.word.c_str());
    if (sec < _sectionMin) return false;
    if (sec > _sectionMax) return false;

    bool foundFontSz = false;
    for (int i=0; i<_sectionFontSizes.size(); i++)
    {
        if (fabs(wp.fontSz - _sectionFontSizes[i]) < 1)
        {
            foundFontSz = true;
            break;
        }
    }

    if (!foundFontSz) return false;

    // do we already have the section
    if (_wordsSections.size() > 36)
    {
        int idebug = 1;
        return false; // must be a duplicate
    }


    LogTrace("Found section number %d", sec);

    return true;
}

//============================================================================
//============================================================================
void GeoWords::joinWords()
{
    _wordsJoined.clear();

    double epsilon = 1.99; // 1.01;
    // make a copy of the cull list
    LinkedList<WordPos> wordList;
    wordList = _wordsValid;

    while (wordList.iGetNumItems() > 0)
    {
        WordPos wp = wordList.tGetItemFirst();
        wordList.bRemoveFirst();

        WordPos insert = wp;
        while (wordList.iGetNumItems() > 0)
        {
            WordPos wp = wordList.tGetItemFirst();

            /*
            if (wp.word == "Kenneth" || wp.word == "Kingma")
            {
                if (insert.word == "Kenneth" || insert.word == "Kingma")
                {
                    int idebug = 1;
                }
            }
            */

            // horizontal stacked word match
            double x = fabs(insert.box.l - wp.box.l);
            double y = fabs(insert.box.b - wp.box.t);
            if (x <= epsilon && y <= epsilon)
            {
                insert.join(wp);
                wordList.bRemoveFirst();
                continue;
            }

            // vertical left to right match
             x = fabs(insert.box.r - wp.box.l);
            double yt = fabs(insert.box.t - wp.box.t);
            double yb = fabs(insert.box.b - wp.box.b);
            if ( (yt <= epsilon || yb <= epsilon) && x <= epsilon)
            {
                insert.join(wp);
                wordList.bRemoveFirst();
                continue;
            }

            break;
        }

        _wordsJoined.push_back(insert);
    }
}

//============================================================================
// this is required because some bounding box modes we don't compute the height and only have the top or bottom
// so after computing the boudning box of the geojson and transforming into word space (based on the scale of our box width here)
// we can fill in the height and get our missing top or bottom bounds
//============================================================================
bool GeoWords::updateBoxHeightIfNeeded(double h)
{
    switch (_boxCompute)
    {
    case BoxCompute_Auto:
    case BoxCompute_LRT:
        _box.b = _box.t - h;
        return true;
        break;
    case BoxCompute_LRB:
        _box.t = _box.b + h;
        return true;
        break;
    default:
    //case BoxCompute_TLBR:
        // should already be good
        break;
    }

    return false;
}

//============================================================================
//============================================================================
bool GeoWords::computeWordBox()
{
    const char *func = "GeoWords::computeWordBox() - ";

    bool res = false;
    switch (_boxCompute)
    {
    case BoxCompute_Auto:
        res = computeWordBoxAuto();
        break;
    case BoxCompute_LRT:
        res = computeWordBoxLRT();
        break;
    case BoxCompute_LRB:
        res = computeWordBoxLRB();
        break;
    case BoxCompute_TLBR:
        res = computeWordBoxTLBR();
        break;
    }

    if (res) return true;

    LogTrace("%s failed to compute box of interest, so using entire word list to compute box", func);

    _box.reset();
    for (unsigned int i=0; i< _words.size(); i++)
    {
        _box.update(_words[i].box);
    }

    return false;

    /*
    WordPos *posTL = findTopLeft();
    WordPos *posBR = findBtmRight();

    if (posTL && posBR)
    {
        _box.l = posTL->box.l;
        _box.t = posTL->box.b; // yes we want the bottom of the box cause text is usually on top of the rectangle of interest so get close to with bottom
        _box.r = posBR->box.l; // yes we want left of the right text
        _box.b = posBR->box.b; // yes we want bottom because right text is usually vertical and bottom will match
        return true;
    }

    std::string msg;
    if (!posTL)
    {
        msg = " top left word: ";
        msg += _wordTL;
        msg += ", ";
    }
    else
    {
        msg = " bottom right word: ";
        msg += _wordBR;
        msg += ", ";
    }

    LogTrace("%s failed to find %s to compute box of interest, so using entire word list to compute box", func, msg.c_str());

    for (unsigned int i=0; i< _words.size(); i++)
    {
        _box.update(_words[i].box);
    }

    return false;
    */
}

//============================================================================
//============================================================================
bool GeoWords::computeWordBoxAuto()
{
    const char *func = "GeoWords::computeWordBoxAuto() - ";

    _box.reset();

    // init the box
    for (unsigned int i=0; i< _words.size(); i++)
    {
        _box.update(_words[i].box);
    }

    WordPos *posTN = findTN();
    WordPos *posRW = findRW();

    bool valid = true;
    std::string msg;
    if (!posTN)
    {
        msg = "T-#-N";
        valid = false;
    }
    if (!posRW)
    {
        if (msg.size() > 0) msg += ", ";
        msg += "R-#-W";
        valid = false;
    }

    if (!valid)
    {
        LogTrace("%s failed to find %s to compute box of interest, so using entire word list to compute box", func, msg.c_str());
        return false;
    }

    // all we can find is left, top and right, but we don't need the bottom,
    //  should be able to figure it out from the ratio of the text width to the geojson bounding box width
    _box.l = posTN->box.l + _autoOfsL;
    _box.t = posTN->box.b + _autoOfsTnB;
    _box.r = posRW->box.r + _autoOfsR;

    // we don't know the actual bottom
    return true;
}

//============================================================================
//============================================================================
bool GeoWords::computeWordBoxLRT()
{
    const char *func = "GeoWords::computeWordBoxLRT() - ";

    _box.reset();

    WordPos *posL = findWordL();
    WordPos *posR = findWordR();
    WordPos *posT = findWordT();

    if (!posL || !posR || !posT)
    {
        LogTrace("%s Unable to compute box because of missing words", func);
        return false;
    }

    setBox(*posL, _wordL, Left);
    setBox(*posR, _wordR, Right);
    setBox(*posT, _wordT, Top);

    // Note: we only have left, top and right, but we don't need the bottom,
    //  should be able to figure it out from the ratio of the text width to the geojson bounding box width


    return true;
}

//============================================================================
//============================================================================
bool GeoWords::computeWordBoxLRB()
{
    const char *func = "GeoWords::computeWordBoxLRB() - ";

    _box.reset();

    WordPos *posL = findWordL();
    WordPos *posR = findWordR();
    WordPos *posB = findWordB();

    if (!posL || !posR || !posB)
    {
        LogTrace("%s Unable to compute box because of missing words", func);
        return false;
    }

    setBox(*posL, _wordL, Left);
    setBox(*posR, _wordR, Right);
    setBox(*posB, _wordB, Btm);

    // Note: we only have left, right, and bottom but we don't need the top,
    //  should be able to figure it out from the ratio of the text width to the geojson bounding box width

    return true;
}

//============================================================================
//============================================================================
bool GeoWords::computeWordBoxTLBR()
{
    const char *func = "GeoWords::computeWordBoxTLBR() - ";

    _box.reset();

    WordPos *posTL = findWordTL();
    WordPos *posBR = findWordBR();

    if (!posTL || !posBR)
    {
        LogTrace("%s Unable to compute box because of missing words", func);
        return false;
    }

    setBox(*posTL, _wordTL, Left);
    setBox(*posBR, _wordBR, Right);
    setBox(*posTL, _wordTL, Top);
    setBox(*posBR, _wordBR, Btm);

    return true;
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findTN()
{
    WordPos *posCur = NULL;

    for (unsigned int i=0; i< _words.size(); i++)
    {
        if (_words[i].word.size() < 5)
        {
            continue;
        }
        if(_words[i].word.at(0) != 'T' || _words[i].word.at(1) != '-' || _words[i].word.at(_words[i].word.size()-1) != 'N')
        {
            continue;
        }

        if (!posCur)
        {
            posCur = &_words[i];
            continue;
        }

        if (_words[i].box.t > posCur->box.t)
        {
            posCur = &_words[i];
            continue;
        }
    }

    return posCur;
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findRW()
{
    WordPos *posCur = NULL;

    for (unsigned int i=0; i< _words.size(); i++)
    {
        if (_words[i].word.size() < 5)
        {
            continue;
        }

        if(_words[i].word.at(0) != 'R' || _words[i].word.at(1) != '-' || _words[i].word.at(_words[i].word.size()-1) != 'W')
        {
            continue;
        }

        if (!posCur)
        {
            posCur = &_words[i];
            continue;
        }

        if (_words[i].box.t > posCur->box.t)
        {
            posCur = &_words[i];
            continue;
        }
    }

    return posCur;
}

//============================================================================
//============================================================================
std::string GeoWords::toString(Side side)
{
    switch (side)
    {
    case Left:
        return std::string("Left");
        break;
    case Top:
        return std::string("Top");
        break;
    case Right:
        return std::string("Right");
        break;
    case Btm:
        return std::string("Btn");
        break;
    }

    return std::string("");
}

//============================================================================
//============================================================================
void GeoWords::setBox(const WordPos &wp, const WordFind &wf, Side side)
{
    if (side == Left)
    {
        if (wf.isOrientV())
        {
            _box.l = wp.box.r; // use the right side of the word box to get closest to the map for a vertical word on the left side of the map
        }
        else
        {
            _box.l = wp.box.l;
        }
    }
    else if (side == Right)
    {
        if (wf.isOrientV())
        {
            _box.r = wp.box.l; // use the left side of the word box to get closest to the map for a vertical word on the right side of the map
        }
        else
        {
            _box.r = wp.box.r;
        }
    }
    else if (side == Top)
    {
        if (wf.isOrientV())
        {
            _box.t = wp.box.t;
        }
        else
        {
            _box.t = wp.box.b; // use the btm side of the word box to get closest to the map for a horizontal word on the top of the map
        }
    }
    else if (side == Btm)
    {
        if (wf.isOrientV())
        {
            _box.b = wp.box.b;
        }
        else
        {
            _box.b = wp.box.t; // use the top side of the word box to get closest to the map for a horizontal word on the btm of the map
        }
    }
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWord(const WordFind &w, Side side)
{
    const char *func = "GeoWords::findWord() -";

    if (w.word.size() <= 0)
    {
        LogTrace("%s UnExpected word is empty for .", func, toString(side).c_str());
        return NULL;
    }

    WordPos *posCur = NULL;
    for (unsigned int i=0; i< _words.size(); i++)
    {
        int found = (int)_words[i].word.find("UNION");
        if (found!=std::string::npos)
        {
            //int idebug = 1;
        }

        if (_words[i].word != w.word)
        {
            continue;
        }

        if (!posCur)
        {
            posCur = &_words[i];
            LogTrace("%s word found at location: %d", w.word.c_str(), i);
            continue;
        }

        switch(side)
        {
        case Left:
            if (_words[i].box.l < posCur->box.l)
            {
                posCur = &_words[i];
                LogTrace("%s word found at location: %d", w.word.c_str(), i);
            }
            break;
        case Right:
            if (_words[i].box.r > posCur->box.r)
            {
                posCur = &_words[i];
                LogTrace("%s word found at location: %d", w.word.c_str(), i);
            }
            break;
        case Top:
            if (_words[i].box.t > posCur->box.t)
            {
                posCur = &_words[i];
                LogTrace("%s word found at location: %d", w.word.c_str(), i);
            }
            break;
        case Btm:
            if (_words[i].box.b < posCur->box.b)
            {
                posCur = &_words[i];
                LogTrace("%s word found at location: %d", w.word.c_str(), i);
            }
            break;
        }
    }

    return posCur;
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWordL()
{
    return findWord(_wordL, Left);
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWordR()
{
    return findWord(_wordR, Right);
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWordT()
{
    return findWord(_wordT, Top);
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWordB()
{
    return findWord(_wordB, Btm);
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWordTL()
{
    WordPos *posCur = NULL;

    for (unsigned int i=0; i< _words.size(); i++)
    {
        if (_words[i].word != _wordTL.word)
        {
            continue;
        }

        if (!posCur)
        {
            posCur = &_words[i];
            continue;
        }

        //if (_words[i].box.t > posCur->box.t || _words[i].box.l < posCur->box.l)
        if (_words[i].box.t > posCur->box.t)
        {
            posCur = &_words[i];
            continue;
        }
    }

    return posCur;
}

//============================================================================
//============================================================================
GeoWords::WordPos* GeoWords::findWordBR()
{
    WordPos *posCur = NULL;

    for (unsigned int i=0; i< _words.size(); i++)
    {
        if (_words[i].word != _wordBR.word)
        {
            continue;
        }

        if (!posCur)
        {
            posCur = &_words[i];
            continue;
        }

        //if (_words[i].box.b < posCur->box.b || _words[i].box.r > posCur->box.r)
        if (_words[i].box.b < posCur->box.b)
        {
            posCur = &_words[i];
            continue;
        }
    }

    return posCur;
}

//============================================================================
//============================================================================
void GeoWords::computeBox(DrawData *pdd, box3d *pbox)
{
    GlObj::computeBox(pdd, pbox);

    pbox->UpdateBox(_box.l, _box.t, 0);
    pbox->UpdateBox(_box.r, _box.b, 0);
}
