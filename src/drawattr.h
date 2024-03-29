#ifndef DRAWATTR_H
#define DRAWATTR_H

#include "gltext.h"
#include "rgbf.h"
#include "colorramp.h"
#include "colorrand.h"

class GlObj;

class DrawAttr
{
public:
    enum LabelPos
    {
        Top = 1,
        Middle = 2,
        Bottom = 4,
        Left = 8,
        Center = 16,
        Right = 32
    };

	enum LineMode
	{
		LM_BASIC = 1,
		LM_SHADER = 2,
		LM_VASER = 4
	};

public:
    DrawAttr();

    bool haveColorByFeature();
    bool haveColorByFeaturePolyFill();
    bool haveRampColor();
    bool haveRampColorPolyFill();
    bool haveRampColorPolyOutline();
    bool haveRampColorLabels();
    bool haveRandColor();
    bool haveRandColorPolyFill();
    bool haveRandColorPolyOutline();
    bool haveRandColorLabels();

    Rgbf getColorPolyFill(const std::string& feature);
    Rgbf getColorPolyFill();
    Rgbf getColorPolyOutline();
    Rgbf getColorLabels();

    void addFeatureColor(const std::string &feature, const std::string &color);
    
    static bool drawPolyFill(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static bool drawPolyOutline(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static bool drawPolyMasked(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static bool drawLabels(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);

    static float alpha(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static Rgbf colorPolyFill(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static Rgbf colorPolyOutline(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static Rgbf colorLabels(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);

    static const vector< shared_ptr<GlObj> >* maskPoly(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);

    static float lineWidth(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static bool lineAA(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
	static int lineMode(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);

    static GlText* font(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);
    static std::string* feature(shared_ptr<DrawAttr> plyr, shared_ptr<DrawAttr> pover);

    static bool drawPolyFill(DrawAttr *plyr, DrawAttr *pover);
    static bool drawPolyOutline(DrawAttr *plyr, DrawAttr *pover);
    static bool drawPolyMasked(DrawAttr *plyr, DrawAttr *pover);

    static bool drawLabels(DrawAttr *plyr, DrawAttr *pover);

    static float alpha(DrawAttr *plyr, DrawAttr *pover);
    static Rgbf colorPolyFill(DrawAttr *plyr, DrawAttr *pover);
    static Rgbf colorPolyOutline(DrawAttr *plyr, DrawAttr *pover);
    static Rgbf colorLabels(DrawAttr *plyr, DrawAttr *pover);

    static const vector< shared_ptr<GlObj> >* maskPoly(DrawAttr *plyr, DrawAttr *pover);

    static float lineWidth(DrawAttr *plyr, DrawAttr *pover);
    static bool lineAA(DrawAttr *plyr, DrawAttr *pover);
	static int lineMode(DrawAttr *plyr, DrawAttr *pover);

    static GlText* font(DrawAttr *plyr, DrawAttr *pover);
    static std::string* feature(DrawAttr *plyr, DrawAttr *pover);




public:
    std::string _name;

    bool _drawName;
    bool _drawLabels;
    std::string _feature;
    bool _useLabelPosOffset;
    Point2d _labelOfs;

    bool _drawPolyFill;
    bool _drawPolyOutline;
    bool _drawPolyMasked;
    vector< shared_ptr<GlObj> > _maskPolyList;

    // TODO: this can be flushed out much better
    float _alpha;
    Rgbf _colorPolyFill;
    Rgbf _colorPolyOutline;
    Rgbf _colorLabels;

    PColorRamp _colorRampPolyFill;
    PColorRamp _colorRampPolyOutline;
    PColorRamp _colorRampLabels;

    PColorRand _colorRandPolyFill;
    PColorRand _colorRandPolyOutline;
    PColorRand _colorRandLabels;

    bool _colorByFeaturePolyFill;
    std::map<std::string, Rgbf> _colorFeatureMap;

    float _lineWidth;
    bool _lineAA;
	int _lineMode;

    GlText _font;

    bool _overrideDrawLabels;

    bool _overrideDrawPolyFill;
    bool _overrideDrawPolyOutline;
    bool _overrideDrawPolyMasked;

    bool _overrideAlpha;
    bool _overrideColorPolyFill;
    bool _overrideColorPolyOutline;
    bool _overridePolyMask;
    bool _overrideColorLabels;

    bool _overrideLineWidth;
    bool _overrideLineAA;
	bool _overrideLineMode;

    bool _overrideFont;
    bool _overrideFeature;

    bool _fillFeatureColorMap;
    typedef std::multimap<std::string, std::string> FeatureColorMap; // feature, color
    FeatureColorMap _filledFeatureColorMap;
};

typedef std::shared_ptr< DrawAttr > PDrawAttr;

#endif
