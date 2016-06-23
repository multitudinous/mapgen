#include "legend.h"
#include "logger.h"
#include "utlmath.h"
#include <QtSvg/QtSvg>
#include <QtSvg/QSvgGenerator>

//============================================================================
//============================================================================
Legend::Legend(const Font *fiTitle, const Font *fiValue) :
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

    _fontTitle.reset(new QFont(fiTitle->face().c_str()));
    _fontTitle->setPixelSize(fiTitle->getPixelSize() * _scale);
    _fontTitle->setWeight(QFont::Bold);

    _fontValue.reset(new QFont(fiValue->face().c_str()));
    _fontValue->setPixelSize(fiValue->getPixelSize() * _scale);
    _fontValue->setWeight(QFont::Bold);

    _colorTitle = fiTitle->color();
    _colorValue = fiValue->color();

    /*
    _fontTitle.reset(new QFont("arial", int(14 * _scale)));
    _fontTitle->setWeight(QFont::Bold);

    int szp = UtlQt::computeFontPixelSize(_fontTitle.get(), int(14 * _scale));
    LogTrace("Legend - Pixel size for title font from point size 14 is %d", szp);


    _fontValue.reset(new QFont("arial", int(12 * _scale)));
    _fontValue->setWeight(QFont::Bold);

    szp = UtlQt::computeFontPixelSize(_fontValue.get(), int(12 * _scale));
    LogTrace("Legend - Pixel size for value font from point size 12 is %d", szp);
    */

    /*
    your render DPI variable should be 96 for Windows and 72 for OSX
    according to: http://www.rfwilmut.clara.net/about/fonts.html
    On a Macintosh monitor, the notional resolution is 72 dots-per -inch (dpi), so that a graphic 72 pixels wide would notionally be 1 inch wide - though obviously the actual size would depend on the individual monitor. However it will always print one inch wide.
    But on a Windows monitor the resolution is (usually) 96 dpi. This means that though the picture is still 72 pixels wide, it will print at 0.75 inches.
    */
}


//============================================================================
//============================================================================
Legend::~Legend()
{

}

//============================================================================
//============================================================================
bool Legend::init(const std::string &file, const std::string &legtype, const std::string &legformat, PColorRamp colorRamp, const std::string &dataObjName, double min, double mid, double max, int decimals, std::string units, const std::string &custom_units)
{
    const char *func = "Legend::init() -";

    _validType = false;
    _file = file;
    _legType = legtype;
    _legFrmt = legformat;
    _catColors.reset();

    if (legtype == "dem")
        initDem(min, mid, max, units);
    else if (legtype == "yield")
        initYield(min, mid, max, decimals, custom_units);
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
    else if (legtype == "cdl")
    {
        if (!initCdl(colorRamp, dataObjName))
        {
            return false;
        }
    }
    else if (legtype == "rr")
        initRR();
    else if (legtype == "r2d2")
        initRUSLE22D();
    else if (legtype == "generic_bucket")
    {
        if (!initGenericBucket(colorRamp, custom_units, decimals))
        {
            return false;
        }
    }
    else if (legtype == "bucket_2")
        initBucket2(colorRamp, custom_units, decimals);
    else if (legtype == "bucket_3")
        initBucket3(colorRamp, custom_units, decimals);
    else
    {
        LogError("%s unknown function type %s", func, _legType.c_str());
        return false;
    }

    initColorRamp(colorRamp);

    _validType = true;
    return true;
}

//============================================================================
//============================================================================
void Legend::initColorRamp(PColorRamp colorRamp)
{
    if (!colorRamp) return;
    if (!isStandardBucketType()) return;

    _min = colorRamp->_minv;
    _max = colorRamp->_maxv;
    _buckets = colorRamp->_buckets;
    _colorMin = colorRamp->_picker->getMin();
    _colorMid = colorRamp->_picker->getMid();
    _colorMax = colorRamp->_picker->getMax();
}

//============================================================================
//============================================================================
bool Legend::isStandardBucketType()
{
    if (_legType == "toteros" || _legType == "winderos" || _legType == "watereros" ||
        _legType == "sci" || _legType == "sciom" || _legType == "profit" ||
        _legType == "rr" || _legType == "r2d2")
    {
        return true;
    }

    return false;
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
bool Legend::initCdl(PColorRamp colorRamp, const std::string &dataObjName)
{
    LogTrace("creating cdl legend ...");

    _title = "Crops";

    if (!colorRamp)
    {
        LogError("Legend::initCdl - must have a color ramp");
        return false;
    }

    ColorPickerCat *picker = dynamic_cast<ColorPickerCat *>(colorRamp->getPicker().get());
    if (picker == NULL)
    {
        LogError("Legend::initCdl - must have a ColorPickerCat type for the color ramp");
        return false;
    }

    _catColors = picker->getColorUsedVec(dataObjName.c_str());
    if (!_catColors)
    {
        LogError("Legend::initCdl - failed to find categories used for dataobj: %s", dataObjName.c_str());
        return false;
    }

    // todo: need to adjust width and height based on how many buckets vertically, and largest category name horizontally
    std::string legFrmtPrev = _legFrmt;
    _legFrmt == "png";

    _width = 1024;
    _height = 1024;
    box2i box;
    box.update(0, 0);
    drawBegin();
    drawBuckets(true, &box);

    _width = box.width() + _mDraw*2 + 4; // add margins, its coming up a bit short for some reason, so adding  on a bit extra
    _height = box.height() + _mDraw * 2 + 4; // add margins, its coming up a bit short for some reason, so adding on a bit extra
    _legFrmt = legFrmtPrev;


    return true;
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
void Legend::initYield(double min, double mid, double max, int decimals, const std::string &custom_units)
{
    LogTrace("creating yield legend ...");

    if ("" == custom_units)
    {
        _title = "Yield (bu/ac)";
    }
    else
    {
        _title = custom_units;
    }
    _min = min;
    _mid = mid;
    _max = max;
    _decimals = decimals;
    _width = 170;
    _height = 300;
}

//============================================================================
//============================================================================
void Legend::initBreakevenCom(double min, double mid, double max)
{
    LogTrace("creating breakeven commodity legend ...");

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
bool Legend::initGenericBucket(PColorRamp colorRamp, const std::string &custom_units, int decimals)
{
    if (!colorRamp)
    {
        LogError("Legend::initGenericBucket - must have a color ramp");
        return false;
    }

    _min = colorRamp->_minv;
    _max = colorRamp->_maxv;
    _buckets = colorRamp->_buckets;
    _incAmt = (_max - _min) / static_cast<double>(_buckets);
    _colorMin = colorRamp->_picker->getMin();
    _colorMid = colorRamp->_picker->getMid();
    _colorMax = colorRamp->_picker->getMax();
    _decimals = decimals;
    _minTop = false;
    _useMaxPlus = false;
    _useLess = false;
    _useGreater = false;
    _showRange = true;
    _title = custom_units;

    return true;
}

//============================================================================
//============================================================================
void Legend::initBucket2(PColorRamp colorRamp, const std::string &custom_units, int decimals)
{
    _min = colorRamp->_minv;
    _max = colorRamp->_maxv;
    _buckets = colorRamp->_buckets;
    _incAmt = (_max - _min) / (static_cast<double>(_buckets) - 1.0);
    _colorMin = colorRamp->_picker->getMin();
    _colorMid = colorRamp->_picker->getMid();
    _colorMax = colorRamp->_picker->getMax();
    _decimals = decimals;
    _minTop = false;
    _useMaxPlus = false;
    _useLess = true;
    _useGreater = true;
    _showRange = false;
    _title = custom_units;
}


//============================================================================
//============================================================================
void Legend::initBucket3(PColorRamp colorRamp, const std::string &custom_units, int decimals)
{
    _min = colorRamp->_minv;
    _max = colorRamp->_maxv;
    _buckets = colorRamp->_buckets;
    _incAmt = (_max - _min) / (static_cast<double>(_buckets) - 1.0);
    _colorMin = colorRamp->_picker->getMin();
    _colorMid = colorRamp->_picker->getMid();
    _colorMax = colorRamp->_picker->getMax();
    _decimals = decimals;
    _minTop = false;
    _useMaxPlus = false;
    _useLess = false;
    _useGreater = false;
    _showRange = false;
    _title = custom_units;
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
    _painter.reset();
    _renderObj.reset();

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
    _painter->setRenderHint(QPainter::TextAntialiasing);
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
    QRect rcDraw = UtlQt::rectWithMargins(left, top, width, height, _mDraw, _mDraw, _mDraw, _mDraw);
    QRect rcTitle = getRcTitle(rcDraw);
    int barl = rcDraw.left() + sint(_mDataL);
    int bart = rcTitle.bottom() + sint(_mDataT);
    int barw = sint(30);
    int barh = rcDraw.height() - (sint(_mDataT) + sint(_mDataB)) - rcTitle.height();
    QRect rcBar = QRect(barl, bart, barw, barh);

    double min1, min2, mid1, mid2; // user wants 4 more values, 2 below the mid, and 2 above

    double dism = mid - min;
    dism = dism / 3.0;
    double disd = max - mid;
    disd = disd / 3.0;

    min1 = min + dism;
    min2 = min1 + dism;
    mid1 = mid + disd;
    mid2 = mid1 + disd;

    std::vector<double> valsd = { min, min1, min2, mid, mid1, mid2, max };
    std::vector<QString> valss;
    for (size_t i = 0; i < valsd.size(); i++)
    {
        QString vals = rndValToStr(valsd[i]);
        valss.push_back(vals);
    }



    // format our values for display to only 2 decimals
    /*
    QString sbtm = rndValToStr(min);
    QString sbtm1 = rndValToStr(min1);
    QString sbtm2 = rndValToStr(min2);
    QString smid = rndValToStr(mid);
    QString smid1 = rndValToStr(mid1);
    QString smid2 = rndValToStr(mid2);
    QString stop = rndValToStr(max);
    */





    // compute value text position
    int txH = UtlQt::textHeight(*_fontValue);
    int txX = rcBar.right() + sint(_mDataR);
    int txW = rcDraw.right() - txX;
    LogTrace("text width: %d", txW);

    /*
    QRect rcTxMin = UtlQt::textRectVertTop(txX, rcBar.top(), txW, txH); // bottom left pt
    QRect rcTxMid = UtlQt::textRectVertCtr(txX, rcBar.center().y(), txW, txH);
    QRect rcTxMax = UtlQt::textRectVertBtm(txX, rcBar.bottom(), txW, txH);
    */

    LogTrace("bar cy: %d", rcBar.center().y());
    //LogTrace("text mid: %d", rcTxMid);

    // draw title
    _painter->setPen(QPen(_colorTitle));
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
    _painter->setPen(QPen(_colorValue));



    // lets find the maximum width and align to the right based on that
    QFontMetrics fm = _painter->fontMetrics();
    int maxw = 0;
    for (size_t i = 0; i < valss.size(); i++)
    {
        QString text = valss[i];
        int width = fm.width(text);
        if (width > maxw) maxw = width;
    }

    // draw the text values
    int mv = rcBar.height() / ((int)valss.size() - 1);
    int cury = rcBar.bottom();
    int btmtopofs = 4; // shift the bottom and top items a bit to align better with the top and bottom of the bar, we could just center them, but it looks better if the text doesn't go above or below the bar
    for (size_t i = 0; i < valss.size(); i++)
    {
        QString text = valss[i];
        QRect rcTx = UtlQt::textRectVertCtr(txX, cury, maxw, txH);

        int valign = Qt::AlignVCenter;
        if (i == 0)
        {
            rcTx.setBottom(rcBar.bottom() + 4);
            rcTx.setTop(rcTx.bottom() - txH);
            valign = Qt::AlignBottom;
        }
        else if (i == valss.size() - 1)
        {
            rcTx.setTop(rcBar.top() - 4);
            rcTx.setBottom(rcTx.top() + txH);
            valign = Qt::AlignTop;
        }

        _painter->drawText(rcTx, Qt::AlignRight | valign, text);


        cury -= mv;
    }
    /*
    _painter->drawText(rcTxMin, Qt::AlignLeft | Qt::AlignTop, stop);
    _painter->drawText(rcTxMid, Qt::AlignLeft | Qt::AlignVCenter, smid);
    _painter->drawText(rcTxMax, Qt::AlignLeft | Qt::AlignBottom, sbtm);
    */
}

//============================================================================
//============================================================================
void Legend::drawBuckets(bool measure, box2i *box)
{
    LogTrace("drawing buckets legend ...");

    int txH = UtlQt::textHeight(*_fontValue);

    // compute title text height
    QRect rcDraw = UtlQt::rectWithMargins(0, 0, getW(), getH(), _mDraw, _mDraw, _mDraw, _mDraw);
    QRect rcTitle = getRcTitle(rcDraw);

    // draw title
    _painter->setFont(*_fontTitle);
    _painter->setPen(QPen(_colorTitle));
    drawText(rcTitle, QString(_title.c_str()), Qt::AlignLeft | Qt::AlignTop, !measure, measure, box);

    // set up buckets
    _painter->setFont(*_fontValue);
    int bulft = rcDraw.left() + sint(_mDataL);
    int butop = rcTitle.bottom() + sint(_mDataT);

    if (_catColors)
    {
        drawBucketsCat(rcDraw, rcTitle, txH, measure , box);
        return;
    }

    // set up first value inc amount and first color
    double vcur = rndVal(_max);
    double inc = -1 * _incAmt;
    QColor cb = _colorMax;
    QColor ce = _colorMin;
    PQImage gradient;
    if (_minTop)
    {
        vcur = rndVal(_min);
        inc = _incAmt;
        cb = _colorMin;
        ce = _colorMax;
    }

    // set up a gradient picker
    gradient = UtlQt::gradientPicker(cb, _colorMid, ce);

    // Make room for > bucket and < bucket
    if (_useGreater && _useLess)
        _buckets += 2;

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

        // next-to-last item
        if (num == _buckets - 1)
        {
            if (_useGreater && _useLess)
                incvalue = false;
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
        int bubtm = drawBucket(rcDraw, bulft, butop, color, strout, txH, measure, box);


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
void Legend::drawBucketsCat(QRect rcDraw, QRect rcTitle, int txH, bool measure, box2i *box)
{
    int bulft = rcDraw.left() + sint(_mDataL);
    int butop = rcTitle.bottom() + sint(_mDataT);

    for (size_t i = 0; i < _catColors->size(); i++)
    {
        ColorPickerCat::PCat cat = _catColors->at(i);
        QColor color = cat->c;
        QString name = QString::fromStdString(cat->name);

        // draw the bucket
        int bubtm = drawBucket(rcDraw, bulft, butop, color, name, txH, measure, box);

        // jump to top of next bucket
        butop = bubtm + sint(_mBucketY);
    }
}

//============================================================================
//============================================================================
int Legend::drawBucket(const QRect &rcDraw, int bleft, int btop, const QColor &bcolor, const QString &text, int txh, bool measure, box2i *box)
{
    QRect rcBucket = QRect(bleft, btop, int(_bucketW * _scale), int(_bucketH * _scale));

    int txleft = rcBucket.right() + int(_mDataR * _scale);
    int txcy = rcBucket.center().y() + 1;
    int txwidth = rcDraw.right() - txleft;

    // need to account for the pen outline
    QRect rcText = QRect(txleft, rcBucket.top() - 1, txwidth, rcBucket.height() + 2);

    // draw the rect filled with color and black outline
    if (measure)
    {
        box->update(rcBucket.left(), rcBucket.top());
        box->update(rcBucket.right(), rcBucket.bottom());
        drawText(rcText, text, Qt::AlignLeft | Qt::AlignVCenter, false, true, box);
    }
    else
    {

        QBrush br(bcolor);
        _painter->setBrush(br);
        QPen pen(QColor(0, 0, 0));
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(sint(2));
        _painter->setPen(pen);
        _painter->drawRect(rcBucket);

        _painter->setPen(QPen(_colorValue));
        _painter->drawText(rcText, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    // draw text box
    //_painter->drawLine(rcText.topLeft(), rcText.bottomLeft());
    // _painter->drawLine(rcText.bottomLeft(), rcText.bottomRight());
    // _painter->drawLine(rcText.bottomRight(), rcText.topRight());
    // _painter->drawLine(rcText.topRight(), rcText.topLeft());

    return rcBucket.bottom();
}

//============================================================================
//============================================================================
void Legend::drawText(const QRect &rcText, const QString &text, int flags, bool draw, bool measure, box2i *box)
{
    if (draw)
    {
        _painter->drawText(rcText, flags, text);
    }

    if (measure && box)
    {
        QRect rc = _painter->boundingRect(rcText, flags, text);
        box->update(rc.left(), rc.top());
        box->update(rc.right(), rc.bottom());
    }
}

/*
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
*/
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
