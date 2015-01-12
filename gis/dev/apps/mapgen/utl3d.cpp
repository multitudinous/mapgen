#include "utl3d.h"
#include "utlmath.h"

double Utl3d::Angle(vec3f v1, vec3f v2, bool normalize, bool deg)
{
	if (normalize)
	{
		v1.Normalize();
		v2.Normalize();
	}

	float dot = DotProduct(v1, v2);

	if (deg)
		return UtlMath::degArcCos(dot);
	else
		return UtlMath::radArcCos(dot);
}

float Utl3d::Anglef(vec3f v1, vec3f v2, bool normalize, bool deg)
{
	return (float)Angle(v1, v2, normalize, deg);
}

void Utl3d::MatrixConcatenate(float *result, float *ma, float *mb)
{
    int i;
    double mb00, mb01, mb02, mb03,
           mb10, mb11, mb12, mb13,
           mb20, mb21, mb22, mb23,
           mb30, mb31, mb32, mb33;
    double mai0, mai1, mai2, mai3;
    
    mb00 = mb[0];  mb01 = mb[1];
    mb02 = mb[2];  mb03 = mb[3];
    mb10 = mb[4];  mb11 = mb[5];
    mb12 = mb[6];  mb13 = mb[7];
    mb20 = mb[8];  mb21 = mb[9];
    mb22 = mb[10]; mb23 = mb[11];
    mb30 = mb[12]; mb31 = mb[13];
    mb32 = mb[14]; mb33 = mb[15];

    for (i = 0; i < 4; i++) {
        mai0 = ma[i*4+0];  mai1 = ma[i*4+1];
	    mai2 = ma[i*4+2];  mai3 = ma[i*4+3];

        result[i*4+0] = (float) (mai0 * mb00 + mai1 * mb10 + mai2 * mb20 + mai3 * mb30);
        result[i*4+1] = (float) (mai0 * mb01 + mai1 * mb11 + mai2 * mb21 + mai3 * mb31);
        result[i*4+2] = (float) (mai0 * mb02 + mai1 * mb12 + mai2 * mb22 + mai3 * mb32);
        result[i*4+3] = (float) (mai0 * mb03 + mai1 * mb13 + mai2 * mb23 + mai3 * mb33);
    }
}