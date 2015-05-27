#include "legend.h"
#include "logger.h"
#include "utlmath.h"
#include <QtSvg>
#include <QSvgGenerator>

//============================================================================
//============================================================================
Legend::Legend() :
_legType("dem"),
_legFrmt("svg"),
_units("m"),
_title(""),
_min(0.0),
_max(5.0),
_mid(0.0),
_incAmt(0.5),
_decimals(1),
_minTop(true),
_useMaxPlus(false),
_useLess(false),
_useGreater(false),
_showRange(true),
_colorMin("#55aa00"),
_colorMid("#ffff00"),
_colorMax("#ff0000"),
_buckets(11),
_bucketW(30),
_bucketH(20),
_width(190),
_height(350),
_file("legend.svg"),
_mTitleT(2),
_mDraw(2),
_mDataL(5),
_mDataT(5),
_mDataR(10),
_mDataB(5),
_mBucketY(10),
_scale(1.0),
_validType(false)
{
    _fontTitle.reset(new QFont("arial", int(14 * _scale)));
    _fontTitle->setWeight(QFont::Bold);

    _fontValue.reset(new QFont("arial", int(12 * _scale)));
    _fontValue->setWeight(QFont::Bold);
}


//============================================================================
//============================================================================
Legend::~Legend()
{

}

//============================================================================
//============================================================================
bool Legend::init(const std::string &file, const std::string &legtype, const std::string &legformat, double min, double mid, double max, std::string units)
{
    const char *func = "Legend::init() -"; 

    _validType = false;
    _file = file;
    _legType = legtype;
    _legFrmt = legformat;

    if (legtype == "dem")
        initDem(min, mid, max, units);
    else if (legtype == "yield")
        initYield(min, mid, max);
    else if (legtype == "soilloss")
        initSoilLoss(min, mid, max);
    else if (legtype == "sedload")
        initSedLoad(min, mid, max);
    else if (legtype == "brkevcom")
        initBreakevenCom(min, mid, max);
    else if (legtype == "brkevexp")
        initBreakevenExp(min, mid, max);
    else if (legtype == "toteros")
        initToteros();
    else if (legtype == "winderos")
        initWinderos();
    else if (legtype == "watereros")
        initWatereros();
    else if (legtype == "sci")
        initSci();
    else if (legtype == "sciom")
        initSciom();
    else if (legtype == "profit")
        initProfit();
    else if (legtype == "rr")
        initRR();
    else if (legtype == "r2d2")
        initRUSLE22D();
    else
    {
        LogError("%s unknown function type %s", func, _legType.c_str());
        return false;
    }

    _validType = true;
    return true;
}

//============================================================================
//============================================================================
bool Legend::render()
{
    if (!_validType) return false;

    drawBegin();
    draw();
    if (!drawEnd())
    {
        return false;
    }

    return true;
}

//============================================================================
//============================================================================
void Legend::setColorMin(const QColor &c)
{
    _colorMin = c;
}

//============================================================================
//============================================================================
void Legend::setColorMid(const QColor &c)
{
    _colorMid = c;
}

//============================================================================
//============================================================================
void Legend::setColorMax(const QColor &c)
{
    _colorMax = c;
}

//============================================================================
//============================================================================
void Legend::initToteros()
{
    LogTrace("creating total erosion legend ...");

    _title = "Total Erosion (tn/ac)";
    initSettingsErosion();
}

//============================================================================
//============================================================================
void Legend::initWinderos()
{
    LogTrace("creating wind erosion legend ...");

    _title = "Wind Erosion (tn/ac)";
    initSettingsErosion();
}

//============================================================================
//============================================================================
void Legend::initWatereros()
{
    LogTrace("creating water erosion legend ...");

    _title = "Water Erosion (tn/ac)";
    initSettingsErosion();
}

//============================================================================
//============================================================================
void Legend::initSci()
{
    LogTrace("creating sci legend ...");

    _title = "SCI";
    initSettingsSci();
}

//============================================================================
//============================================================================
void Legend::initSciom()
{
    LogTrace("creating sciom legend ...");

    _title = "SCI-OM";
    initSettingsSci();
}

//============================================================================
//============================================================================
void Legend::initRUSLE22D()
{
    LogTrace("creating RUSLE2 2D legend ...");

    _title = "Soil Loss (tn/ac)";
    initSettingsRUSLE22D();
}

//============================================================================
//============================================================================
void Legend::initProfit()
{
    LogTrace("creating profit legend ...");

    _title = "Profit ($/ac)";
    initSettingsProfit();
}

//============================================================================
//============================================================================
void Legend::initRR()
{
    LogTrace("creating rate of return legend ...");

    _title = "RR (%)";
    initSettingsRR();
}

//============================================================================
//============================================================================
void Legend::initDem(double min, double mid, double max, const std::string &units)
{
    LogTrace("creating dem legend ...");

    if (units == "ft")
    {
        min = UtlMath::mToFt(min);
        mid = UtlMath::mToFt(mid);
        max = UtlMath::mToFt(max);
    }
    else
    {
        //units = "m";
    }

    _title = "Elevation (" + units + ")";
    _min = min;
    _mid = mid;
    _max = max;
    _decimals = 0;
    _width = 130;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initYield(double min, double mid, double max)
{
    LogTrace("creating yield legend ...");

    _title = "Yield (bu/ac)";
    _min = min;
    _mid = mid;
    _max = max;
    _decimals = 2;
    _width = 170;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initBreakevenCom(double min, double mid, double max)
{
    LogTrace("creating breakeven commodity legend ...");

    _title = "Price ($/bu)";
    _min = min;
    _mid = mid;
    _max = max;
    _decimals = 2;
    _width = 170;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initBreakevenExp(double min, double mid, double max)
{
    LogTrace("creating breakeven expense legend ...");

    _title = "Expense ($/ac)";
    _min = min;
    _mid = mid;
    _max = max;
    _decimals = 2;
    _width = 170;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initSoilLoss(double min, double mid, double max)
{
    LogTrace("creating soil loss legend ...");

    _title = "Soil Loss (tn/ac/yr)";
    _min = min;
    _mid = mid;
    _max = max;
    _decimals = 2;
    _width = 170;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initSedLoad(double min, double mid, double max)
{
    LogTrace("creating sediment load legend ...");

    _title = "Sed Load (%)";
    _min = 0;
    _mid = max / 2.0;
    _max = max;
    _decimals = 2;
    _width = 170;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initSettingsErosion()
{
    _min = 0.0;
    _max = 5.0;
    _incAmt = 0.5;
    _decimals = 1;
    _minTop = true;
    _useMaxPlus = true;
    _useLess = false;
    _useGreater = false;
    _showRange = true;
    _colorMin = getColorEndGreen();
    _colorMid = getColorMidYellow();
    _colorMax = getColorEndRed();
    _buckets = 11;
}

//============================================================================
//============================================================================
void Legend::initSettingsSci()
{
    _width = 140;
    _height = 385;
    _min = -1.0;
    _max = 1.0;
    _incAmt = 0.2;
    _decimals = 1;
    _minTop = true;
    _useMaxPlus = false;
    _useLess = true;
    _useGreater = true;
    _showRange = true;
    _colorMin = getColorEndRed();  //colors are flipped compared to erosion
    _colorMid = getColorMidYellow();
    _colorMax = getColorEndGreen();
    _buckets = 12;
}

//============================================================================
//============================================================================
void Legend::initSettingsRUSLE22D()
{
    _min = -5.0;
    _max = 5.0;
    _incAmt = 1.0;
    _decimals = 1;
    _minTop = true;
    _useMaxPlus = false;
    _useLess = true;
    _useGreater = true;
    _showRange = true;
    _colorMin = getColorEndRed();  //colors are flipped compared to erosion
    _colorMid = getColorMidYellow();
    _colorMax = getColorEndGreen();
    _buckets = 12;
}

//============================================================================
//============================================================================
void Legend::initSettingsProfit()
{
    _min = -500.0;
    _max = 500.0;
    _incAmt = 100.0;
    _decimals = 0;
    _minTop = false;
    _useMaxPlus = false;
    _useLess = false;
    _useGreater = false;
    _showRange = false;
    _colorMin = getColorEndRed();  //colors are flipped compared to erosion
    _colorMid = getColorMidYellow();
    _colorMax = getColorEndGreen();
    _buckets = 11;
}

//============================================================================
//============================================================================
void Legend::initSettingsRR()
{
    _min = -100;
    _max = 100;
    _incAmt = 20;
    _decimals = 0;
    _minTop = false;
    _useMaxPlus = false;
    _useLess = false;
    _useGreater = false;
    _showRange = false;
    _colorMin = getColorEndRed();  //colors are flipped compared to erosion
    _colorMid = getColorMidYellow();
    _colorMax = getColorEndGreen();
    _buckets = 11;
}

//============================================================================
//============================================================================
void Legend::drawBegin()
{
    if (_legFrmt == "png")
    {
        QImage *img(new QImage(QSize(getW(), getH()), QImage::Format_ARGB32));
        img->fill(qRgba(0, 0, 0, 0));
        _renderObj.reset(img);
    }
    else
    {
        std::string desc = std::string("Map Legend ") + _title;
       
        QSvgGenerator *svg = new QSvgGenerator();
        svg->setFileName(_file.c_str());
        svg->setSize(QSize(getW(), getH()));
        svg->setViewBox(QRect(0, 0, getW(), getH()));
        svg->setTitle("Legend");
        svg->setDescription(desc.c_str());
        _renderObj.reset(svg);
    }

    initPaint(_renderObj.get());
}

//============================================================================
//============================================================================
bool Legend::drawEnd()
{
    const char *func = "Legend::drawEnd() -";
    _painter->end();

    if (_legFrmt != "png")
    {
        // svg automatically saves
        return true;
    }

    QImage *imgOut = dynamic_cast<QImage *>(_renderObj.get());
    if (!imgOut)
    {
        LogError("%s UnExpected Error: png legend type missing QImage", func);
        return false;
    }

    if (!imgOut->save(QString(_file.c_str()), "png"))
    {   
        LogError("%s UnExpected Error: failed to save legend to file %s", func, _file.c_str());
        return false;
    }

    return true;
}

//============================================================================
//============================================================================
void Legend::draw()
{
    LogTrace("drawing legend ...");

    if (isRampType())
    {
        drawRamp(0, 0, getW(), getH(), _min, _mid, _max, _colorMin, _colorMid, _colorMax);
    }
    else
    {
        drawBuckets();
    }
}

//============================================================================
//============================================================================
void Legend::initPaint(QPaintDevice *pd)
{
    _painter.reset(new QPainter(pd));
    _painter->setFont(*_fontValue);
    _painter->setRenderHint(QPainter::Antialiasing);
}

//============================================================================
//============================================================================
bool Legend::isRampType()
{
    if (_legType == "dem" || _legType == "yield" || _legType == "soilloss" || _legType == "sedload" || _legType == "brkevcom" || _legType == "brkevexp")
    {
        return true;
    }

    return false;
}

//============================================================================
//============================================================================
int Legend::getW()
{
    return sint(_width);
}

//============================================================================
//============================================================================
int Legend::getH()
{
    return sint(_height);
}

//============================================================================
//============================================================================
int Legend::sint(double v)
{
    return (int)(v * _scale);
}

//============================================================================
//============================================================================
int Legend::sint(int v)
{
    return (int)((double)v * _scale);
}

//============================================================================
//============================================================================
double Legend::rndVal(double v)
{
    return UtlMath::rndUp(v, _decimals);
}

//============================================================================
//============================================================================
QString Legend::rndValToStr(double v)
{
    v = rndVal(v);
    return valToStr(v);
}

//============================================================================
//============================================================================
QString Legend::valToStr(double v)
{
    return QString::number(v, 'f', _decimals);
}

//============================================================================
//============================================================================
QRect Legend::getRcDraw(int left, int top, int width, int height)
{
    int m = _mDraw;
    return UtlQt::rectWithMargins(left, top, width, height, m, m, m, m);
}

//============================================================================
//============================================================================
QRect Legend::getRcTitle(const QRect &rcDraw)
{
    int txH = UtlQt::textHeight(*_fontTitle);
    return QRect(rcDraw.left(), rcDraw.top() + _mTitleT, rcDraw.width(), txH);
}

//============================================================================
//============================================================================
void Legend::drawRamp(int left, int top, int width, int height, double min, double mid, double max, const QColor &minc, const QColor &midc, const QColor &maxc)
{
    LogTrace("drawing ramp legend ...");
    LogTrace("min %f, mid %f, max %f", min, mid, max);

    // compute title text height
    QRect rcDraw = UtlQt::rectWithMargins(left, top, width, height, 2, 2, 2, 2);
    QRect rcTitle = getRcTitle(rcDraw); 
    int barl = rcDraw.left() + sint(_mDataL);
    int bart = rcTitle.bottom() + sint(_mDataT);
    int barw = sint(30);
    int barh = rcDraw.height() - (sint(_mDataT) + sint(_mDataB)) - rcTitle.height();
    QRect rcBar = QRect(barl, bart, barw, barh);


    // format our values for display to only 2 decimals
    QString sbtm = rndValToStr(min);
    QString smid = rndValToStr(mid);
    QString stop = rndValToStr(max);


    // compute value text position
    int txH = UtlQt::textHeight(*_fontValue);
    int txX = rcBar.right() + sint(_mDataR);
    int txW = rcDraw.right() - txX;
    LogTrace("text width: %d", txW);

    QRect rcTxMin = UtlQt::textRectVertTop(txX, rcBar.top(), txW, txH); // bottom left pt
    QRect rcTxMid = UtlQt::textRectVertCtr(txX, rcBar.center().y(), txW, txH);
    QRect rcTxMax = UtlQt::textRectVertBtm(txX, rcBar.bottom(), txW, txH);

    LogTrace("bar cy: %d", rcBar.center().y());
    //LogTrace("text mid: %d", rcTxMid);

    // draw title
    _painter->setFont(*_fontTitle);
    _painter->drawText(rcTitle, Qt::AlignLeft | Qt::AlignTop, QString(_title.c_str()));

    // draw gradient bar with black outline
    PQLinearGradient gradient = UtlQt::gradientRamp(rcBar, maxc, midc, minc); // need to flip the colors
    _painter->setBrush(*gradient);

    // set up outline pen
    QPen pen(QColor(0, 0, 0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(sint(2));
    _painter->setPen(pen);

    _painter->drawRect(rcBar);

    // draw value text
    _painter->setFont(*_fontValue);
    _painter->drawText(rcTxMin, Qt::AlignLeft | Qt::AlignTop, stop);
    _painter->drawText(rcTxMid, Qt::AlignLeft | Qt::AlignVCenter, smid);
    _painter->drawText(rcTxMax, Qt::AlignLeft | Qt::AlignBottom, sbtm);
}

//============================================================================
//============================================================================
void Legend::drawBuckets()
{
    LogTrace("drawing buckets legend ...");

    int txH = UtlQt::textHeight(*_fontValue);

    // compute title text height
    QRect rcDraw = UtlQt::rectWithMargins(0, 0, getW(), getH(), 2, 2, 2, 2);
    QRect rcTitle = getRcTitle(rcDraw);

    // draw title
    _painter->setFont(*_fontTitle); 
    _painter->drawText(rcTitle, Qt::AlignLeft | Qt::AlignTop, QString(_title.c_str()));

    // set up buckets
    _painter->setFont(*_fontValue);
    int bulft = rcDraw.left() + sint(_mDataL);
    int butop = rcTitle.bottom() + sint(_mDataT);

    // set up first value inc amount and first color
    double vcur = rndVal(_max);
    double inc = -1 * _incAmt;
    QColor cb = _colorMax;
    QColor ce = _colorMin;
    if (_minTop)
    {
        vcur = rndVal(_min);
        inc = _incAmt;
        cb = _colorMin;
        ce = _colorMax;
    }

    // set up a gradient picker
    PQImage gradient = UtlQt::gradientPicker(cb, _colorMid, ce);

    // draw buckets and text
    int num = 1;
    while (true)
    {
        bool incvalue = true;
        bool showRange = _showRange;
        QString strout = valToStr(vcur);

        // first item
        if (num == 1)
        {
            if (_minTop)
            {
                if (_useLess)
                {
                    strout = QString("< ") + strout;
                    showRange = false;
                    incvalue = false;
                }
            }
            else
            {
                if (_useGreater)
                {
                    strout = QString("> ") + strout;
                    showRange = false;
                    incvalue = false;
                }
                else if (_useMaxPlus)
                {
                    strout = QString("+ ") + strout;
                    showRange = false;
                    incvalue = false;
                }
            }
        }

        // last item
        if (num == _buckets)
        {
            if (_minTop)
            {
                if (_useGreater)
                {
                    strout = QString("> ") + strout;
                    showRange = false;
                }
                else if (_useMaxPlus)
                {
                    strout = QString("+ ") + strout;
                    showRange = false;
                }
            }
            else
            {
                if (_useLess)
                {
                    strout = QString("< ") + strout;
                    showRange = false;
                }
            }
        }


        if (incvalue)
        {
            vcur = vcur + inc; // get next value
            vcur = rndVal(vcur);
        }


        if (showRange)
        {
            strout = strout + QString("  -  ") + valToStr(vcur);
        }

        // get the color
        float perColor = float(num) / float(_buckets);
        QColor color = UtlQt::gradientPick(*gradient, perColor);

        // draw the bucket
        int bubtm = drawBucket(rcDraw, bulft, butop, color, strout, txH);


        // jump to top of next bucket
        butop = bubtm + sint(_mBucketY);

        num = num + 1;
        if (num > _buckets)
        {
            break;
        }
    }
}

//============================================================================
//============================================================================
int Legend::drawBucket(const QRect &rcDraw, int bleft, int btop, const QColor &bcolor, const QString &text, int txh)
{
    QRect rcBucket = QRect(bleft, btop, int(_bucketW * _scale), int(_bucketH * _scale));

    int txleft = rcBucket.right() + int(_mDataR * _scale);
    int txcy = rcBucket.center().y() + 1;
    int txwidth = rcDraw.right() - txleft;

    // need to account for the pen outline
    QRect rcText = QRect(txleft, rcBucket.top() - 1, txwidth, rcBucket.height() + 2);

    // draw the rect filled with color and black outline
    QBrush br(bcolor);
    _painter->setBrush(br);
    QPen pen(QColor(0, 0, 0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(sint(2));
    _painter->setPen(pen);
    _painter->drawRect(rcBucket);
    _painter->drawText(rcText, Qt::AlignLeft | Qt::AlignVCenter, text);

    // draw text box
    //_painter->drawLine(rcText.topLeft(), rcText.bottomLeft());
    // _painter->drawLine(rcText.bottomLeft(), rcText.bottomRight());
    // _painter->drawLine(rcText.bottomRight(), rcText.topRight());
    // _painter->drawLine(rcText.topRight(), rcText.topLeft());

    return rcBucket.bottom();
}

//============================================================================
//============================================================================
QColor Legend::getColorEndGreen()
{
    return QColor("#55aa00");
}

//============================================================================
//============================================================================
QColor Legend::getColorEndRed()
{
    return QColor("#ff0000");
}

//============================================================================
//============================================================================
QColor Legend::getColorMidYellow()
{
    return QColor("#ffff00");
}