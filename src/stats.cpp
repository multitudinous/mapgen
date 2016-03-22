#include "stats.h"
#include "utlmisc.h"
#include "utlmath.h"

const double Stats::SDEV_FULLRANGE = -1;
const double Stats::FORCE_MINMAX_OFF = -9999;

//============================================================================
//============================================================================
Stats::Stats() :
    _min(0),
    _max(0),
    _mean(0),
    _sdev(0),
    _adjustedMin(0),
    _adjustedMax(0),
    _forceSdevs(SDEV_FULLRANGE),
    _forceMin(FORCE_MINMAX_OFF),
    _forceMax(FORCE_MINMAX_OFF),
    _nodataValue(9999)
{

}

//============================================================================
//============================================================================
Stats::Stats(const Stats &s) :
    _min(s._min),
    _max(s._max),
    _mean(s._mean),
    _sdev(s._sdev),
    _adjustedMin(s._adjustedMin),
    _adjustedMax(s._adjustedMax),
    _forceSdevs(s._forceSdevs),
    _forceMin(s._forceMin),
    _forceMax(s._forceMax),
    _nodataValue(s._nodataValue)
{

}

//============================================================================
//============================================================================
double Stats::getMin() const
{
    return _adjustedMin;
}

//============================================================================
//============================================================================
double Stats::getMid() const
{
    return _adjustedMin + (getLen() / 2.0);
}

//============================================================================
//============================================================================
double Stats::getMax() const
{
    return _adjustedMax;
}

//============================================================================
// get the normalized position of the value between min and max (result will be a percent)
//============================================================================
double Stats::getPosNorm(double value, bool clamp) const
{
    double pos = (value - _adjustedMin) / getLen();
    if (clamp)
    {
        pos = UtlMath::clamp<double>(pos, 0, 1);
    }

    return pos;
}

//============================================================================
// get the normalized position of the value between min and max (result will be a percent)
// this uses the original min and max values and not the adjusted ones, usually results in color saturation
// and not a good representation of the data because of outlier influence
//============================================================================
double Stats::getPosNormOrig(double value, bool clamp) const
{
    double pos = (value - _min) / getLenOrig();
    if (clamp)
    {
        pos = UtlMath::clamp<double>(pos, 0, 1);
    }

    return pos;
}

//============================================================================
//============================================================================
double Stats::getLen() const
{
    return _adjustedMax - _adjustedMin;
}

//============================================================================
//============================================================================
double Stats::getLenOrig() const
{
    return _max - _min;
}

//============================================================================
//============================================================================
double Stats::getMean() const
{
    return _mean;
}

//============================================================================
//============================================================================
double Stats::getForceSdevs() const
{
    return _forceSdevs;
}

//============================================================================
//============================================================================
double Stats::getForceMin() const
{
    return _forceMin;
}

//============================================================================
//============================================================================
double Stats::getForceMax() const
{
    return _forceMax;
}

//============================================================================
//============================================================================
double Stats::getMinOrig() const
{
    return _min;
}

//============================================================================
//============================================================================
double Stats::getMaxOrig() const
{
    return _max;
}

//============================================================================
//============================================================================
void Stats::computeAdjusted()
{
    _adjustedMin = _min;
    _adjustedMax = _max;

    adjustFromMean();
    adjustSetValue();
}

//============================================================================
//============================================================================
bool Stats::adjustFromMean()
{
    if (_forceSdevs < 0) return false;

    //double move = _forceSdevs * _mean;
	double move = _forceSdevs * _sdev;
    _adjustedMin = _mean - move;
    _adjustedMax = _mean + move;

    return true;

}

//============================================================================
//============================================================================
bool Stats::adjustSetValue()
{
    bool modified = false;

    if (!UtlMisc::equality(_forceMin, FORCE_MINMAX_OFF))
    {
        _adjustedMin = _forceMin;
        modified = true;
    }

    if (!UtlMisc::equality(_forceMax, FORCE_MINMAX_OFF))
    {
        _adjustedMax = _forceMax;
        modified = true;
    }

    return modified;
}

//============================================================================
//============================================================================
void Stats::copySettings(const Stats &stat)
{
    _forceSdevs = stat._forceSdevs;
    _forceMin = stat._forceMin;
    _forceMax = stat._forceMax;
    computeAdjusted();
}

//============================================================================
//============================================================================
Stats& Stats::operator=(const Stats &s) 
{
    if (this == &s) return *this;

    _min = s._min;
    _max = s._max;
    _mean = s._mean;
    _sdev = s._sdev;

    _adjustedMin = s._adjustedMin;
    _adjustedMax = s._adjustedMax;

    _forceSdevs = s._forceSdevs;
    _forceMin = s._forceMin;
    _forceMax = s._forceMax;

    return *this;
}