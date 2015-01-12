#ifndef GEOCOMPUTE_H
#define GEOCOMPUTE_H

#include "globj.h"
#include "geowords.h"
#include "geopoly.h"
#include <QTextStream>

class GeoCompute : public GlObj
{
protected:
    typedef std::vector<GeoWords::WordPos> WordsList;
    typedef std::tr1::shared_ptr<WordsList> PWordsList;
    typedef std::map<std::string, PWordsList> WordMap;
    typedef std::map<std::string, GeoPoly*> PolyMap;

    struct SecValidation
    {
        int secCount;
        int secFound;
        float perFound;

        SecValidation()
        {
            secCount = 0;
            secFound = 0;
            perFound = 0;
        }
    };

public:
    GeoCompute();

    //void setName(const std::string &name) { _name = name; }
    void setType(const std::string &type) { _type = type; }
    void setOutput(const std::string &output) { _output = output; }
    void setValidationFile(const std::string &output) { _validationFile = output; }
    void setFeature(const std::string &feature) { _feature = feature; }

    virtual void draw(DrawData *pdd);

    virtual void computeBox(DrawData *pdd, box3d *pbox);

    //std::string getName() { return _name; }

    bool compute();

protected:
    bool computeWordFinder();
    void getFeaturePolymap(GlObj *parent, PolyMap &map);
    void validateSections(const WordsList &words, const PolyMap &polys, SecValidation *secVal);
    void locateWords(const WordsList &words, const PolyMap &polys, WordMap &wordMap, WordsList &wordsNoLoc);
    bool locateWord(const GeoWords::WordPos *wp, const PolyMap &polys, PolyMap::const_iterator *pit=NULL);
    void outputWords(const WordMap &wordMap, const WordsList &wordsNoLoc, const PolyMap &polyMap, const SecValidation &secVal);
    void outputValidation(QTextStream &stream, const SecValidation &secVal);
    void outputValidation(const SecValidation &secVal);
    void getPolyVerbose(const PolyMap &polyMap, const std::string &feature, std::string *pOut);
    void getWordVerbose(const GeoWords::WordPos &wp, std::string *pOut);

protected:
    //std::string _name;
    std::string _type;
    std::string _output;
    std::string _validationFile;
    std::string _feature;
    bool _verbose;
    //std::vector<PGlObj> _dataObjs;

    WordMap _wordMap;
};

typedef shared_ptr<GeoCompute> PGeoCompute;
#endif
