#ifndef LEGEND_H
#define LEGEND_H

#include "utlqt.h"

class Legend
{
public:
    Legend();
    virtual ~Legend();

    bool init(const std::string &file, const std::string &legtype, const std::string &legformat, double min = 0, double mid = 0, double max = 0, std::string units = "m");
    bool render();

    void setColorMin(const QColor &c);
    void setColorMid(const QColor &c);
    void setColorMax(const QColor &c);

    static QColor getColorEndGreen();
    static QColor getColorEndRed();
    static QColor getColorMidYellow();

protected:

    void initToteros();
    void initWinderos();
    void initWatereros();
    void initSci();
    void initSciom();
    void initRUSLE22D();
    void initProfit();
    void initRR();
    void initDem(double min, double mid, double max, const std::string &units);
    void initYield(double min, double mid, double max);
    void initBreakevenCom(double min, double mid, double max);
    void initBreakevenExp(double min, double mid, double max);
    void initSoilLoss(double min, double mid, double max);
    void initSedLoad(double min, double mid, double max);
    void initSettingsErosion();
    void initSettingsSci();
    void initSettingsRUSLE22D();
    void initSettingsProfit();
    void initSettingsRR();


    void drawBegin();
    bool drawEnd();
    void draw();

    void drawRamp(int left, int top, int width, int height, double min, double mid, double max, const QColor &minc, const QColor &midc, const QColor &maxc);
    void drawBuckets();
    int drawBucket(const QRect &rcDraw, int bleft, int btop, const QColor &bcolor, const QString &text, int txh);

    void initPaint(QPaintDevice *pd);

    bool isRampType();

    QRect getRcDraw(int left, int top, int width, int height);
    QRect getRcTitle(const QRect &rcDraw);

    int getW();
    int getH();
    int sint(double v);
    int sint(int v);
    double rndVal(double v);
    QString rndValToStr(double v);
    QString valToStr(double v);

public:
    std::string _name;

protected:
    PQPainter _painter;
    PQPaintDevice _renderObj;
    PQFont _fontTitle;
    PQFont _fontValue;

    std::string _legType; // possible types = toteros, winderos, watereros, sci, sciom, profit, dem
    std::string _legFrmt; // possible formats = svg, png
    std::string _units; //'m' // only used for dem type
    std::string _title;
    double _min;        
    double _max;
    double _mid; // only used for dem type
    double _incAmt;
    double _decimals;
    bool _minTop;
    bool _useMaxPlus;
    bool _useLess;
    bool _useGreater;
    bool _showRange;
    QColor _colorMin;
    QColor _colorMid;
    QColor _colorMax;
    int _buckets;
    int _bucketW;
    int _bucketH;
    int _width;
    int _height;
    std::string _file;

    int _mTitleT;   // margin from draw rect to title top
    int _mDraw;     // margin draw rect
    int _mDataL;    // margin from draw rect left to data
    int _mDataT;    // margin title to start of data
    int _mDataR;    // margin from data right to dims
    int _mDataB;    // margin from draw rect left to data
    int _mBucketY;  // space between buckets

    double _scale;

    bool _validType;
};

typedef std::shared_ptr< Legend > PLegend;
#endif