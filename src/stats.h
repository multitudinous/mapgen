#ifndef STATS_H
#define STATS_H

class Stats
{
public:
    static const double SDEV_FULLRANGE;
    static const double FORCE_MINMAX_OFF;

public:
    Stats();
    Stats(const Stats &s);

    // in adjusted values
    double getMin() const;
    double getMid() const;
    double getMax() const;
    double getPosNorm(double value, bool clamp=true) const;
    double getLen() const;

    // refresh adjusted values
    void computeAdjusted();

    void copySettings(const Stats &stat);

    Stats& operator=(const Stats &s);

protected:
    bool adjustFromMean();
    bool adjustSetValue();

public:
    double _min;
    double _max;
    double _mean;
    double _sdev;

    double _adjustedMin;
    double _adjustedMax;

    double _forceSdevs;
    double _forceMin;
    double _forceMax;
};

#endif