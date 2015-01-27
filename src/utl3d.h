#ifndef UTL3D_H
#define UTL3D_H

#include "vector3.h"

class Utl3d
{
public:
	static double Angle(vec3f v1, vec3f v2, bool normalize=true, bool deg=true);
	static float Anglef(vec3f v1, vec3f v2, bool normalize=true, bool deg=true);
	static void MatrixConcatenate(float *result, float *ma, float *mb);
};

// Distance From Plane
#define M_DFP(clPeq,clPt) (clPeq.m_fA*clPt.x + clPeq.m_fB*clPt.y + clPeq.m_fC*clPt.z + clPeq.m_fD)
#define M_DFP2(clPeq,x,y,z) (clPeq.m_fA*x + clPeq.m_fB*y + clPeq.m_fC*z + clPeq.m_fD)

#endif
