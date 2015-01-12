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