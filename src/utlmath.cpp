#include "utlmath.h"

//======================================================
//======================================================
float UtlMath::radArcCos(float value)
{
	float angle;

	angle = acos(value);
	return angle;
}

//======================================================
//======================================================
float UtlMath::degArcCos(float value)
{
	float angle;

	angle = acos(value);
	angle = deg(angle);
	return angle;
}

//======================================================
//======================================================
float UtlMath::deg(float rad)
{
	return rad*f180overpi;
}


//======================================================
//======================================================
float UtlMath::rad(float angle)
{
	return angle*piover180;
}

//======================================================
//======================================================
double UtlMath::saturate(double v)
{
    return clamp<double>(v, 0, 1);
}

//======================================================
//======================================================
float UtlMath::saturate(float v)
{
    return clamp<float>(v, 0, 1);
}

//======================================================
//======================================================
double UtlMath::mToFt(double m)
{
    return m * 3.28084;
}

//======================================================
//======================================================
double UtlMath::rndUp(double v, unsigned int decimals)
{
    long lMul = (long)pow(10.0, (double)decimals);
    double d1 = (v*lMul) - (long)(v*lMul);
    long l1 = (long)(d1 * 10);
    double dAdd = 0;
    if (l1 >= 5)
    {
        dAdd = 1.0 / (double)lMul;
    }

    double dValRnd = ((long)(v*lMul));
    dValRnd /= lMul;
    dValRnd += dAdd;

    return dValRnd;
}