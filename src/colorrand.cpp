#include "colorrand.h"
#include "CDefine.h"
#include "utlmath.h"
#include "randgen.h"
#include <qcolor.h>
#include <ctime>

//============================================================================
//============================================================================
ColorRand::ColorRand()
{
    setHSV(1, 1, 1);
}

//============================================================================
//============================================================================
ColorRand::ColorRand(double s, double v)
{
    /*
    std::srand(std::time(0)); // use current time as seed for random generator
    int random = std::rand();
    double tr1 = (double)random / (double)RAND_MAX;
    random = std::rand();
    double tr2 = (double)random / (double)RAND_MAX;
    */

    RandGen rand;
    double r1 = rand.genRandPercent();
    double r2 = rand.genRandPercent();

    setHSV(r2, s, v);
}

//============================================================================
//============================================================================
ColorRand::ColorRand(double hStart, double s, double v)
{
    setHSV(hStart, s, v);
}

//============================================================================
//============================================================================
void ColorRand::setHSV(double h, double s, double v)
{
    setH(h);
    setS(s);
    setV(v);
}
//============================================================================
//============================================================================
void ColorRand::setH(double d)
{
    _h = UtlMath::saturate(d);
}

//============================================================================
//============================================================================
void ColorRand::setS(double d)
{
    _s = UtlMath::saturate(d);
}

//============================================================================
//============================================================================
void ColorRand::setV(double d)
{
    _v = UtlMath::saturate(d);
}

//============================================================================
//============================================================================
Rgbf ColorRand::getNext()
{
    _h += CDefine::GOLDEN_RATIO_CONJUGATE;
    //_h = 1 - _h;
    _h = fmod(_h, 1.0);

    QColor c;
    c.setHsvF(_h, _s, _v, 1.0);
    Rgbf rgb(c.redF(), c.greenF(), c.blueF(), 1.0f);
    return rgb;
}