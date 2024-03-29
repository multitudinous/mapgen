#ifndef UTLMATH_H
#define UTLMATH_H

#include <math.h>

const float piover180	= 0.01745329252f;
const float f180overpi  =  57.2957795131f;
const float piover2		= 1.57079632679f;
const float sin45		= 0.70710678119f;
const float pi    		= 3.141592654f;

class UtlMath
{
public:
	static float radArcCos(float value);
	static float degArcCos(float value);
	static float deg(float rad);
	static float rad(float angle);

    static double saturate(double v);
    static float saturate(float v);

    template<class T>
    static T clamp(T v, T min, T max)
    {
        return v < min ? min : (v > max ? max : v);
    }

    static double mToFt(double m);
    static double rndUp(double v, unsigned int decimals=2);
};

#endif
