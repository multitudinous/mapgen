#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "vector3.h"
#include <math.h>

template<typename T>
class Matrix4x4
{
public:
	T m_aafMat[4][4]; // note matrix should be [col][row] for opengl, so this matrix is [col][row]

	//====================================================================
	//====================================================================
	Matrix4x4()
	{
	  vIdentity();
	}

	//====================================================================
	//====================================================================
	Matrix4x4(const Matrix4x4 &clMat)
	{
	  m_aafMat[0][0] = clMat.m_aafMat[0][0];
	  m_aafMat[0][1] = clMat.m_aafMat[0][1];
	  m_aafMat[0][2] = clMat.m_aafMat[0][2];
	  m_aafMat[0][3] = clMat.m_aafMat[0][3];
	  m_aafMat[1][0] = clMat.m_aafMat[1][0];
	  m_aafMat[1][1] = clMat.m_aafMat[1][1];
	  m_aafMat[1][2] = clMat.m_aafMat[1][2];
	  m_aafMat[1][3] = clMat.m_aafMat[1][3];
	  m_aafMat[2][0] = clMat.m_aafMat[2][0];
	  m_aafMat[2][1] = clMat.m_aafMat[2][1];
	  m_aafMat[2][2] = clMat.m_aafMat[2][2];
	  m_aafMat[2][3] = clMat.m_aafMat[2][3];
	  m_aafMat[3][0] = clMat.m_aafMat[3][0];
	  m_aafMat[3][1] = clMat.m_aafMat[3][1];
	  m_aafMat[3][2] = clMat.m_aafMat[3][2];
	  m_aafMat[3][3] = clMat.m_aafMat[3][3];
	}

	//====================================================================
	//====================================================================
	~Matrix4x4()
	{
	}

	//====================================================================
	//====================================================================
	void vGet(T *paf16) const 
	{
	  memcpy(paf16, m_aafMat, sizeof(T)*16);
	}

	//====================================================================
	//====================================================================
	void vSet(T *paf16)
	{
	  memcpy(m_aafMat, paf16, sizeof(T)*16);
	}

	//====================================================================
	//====================================================================
	void vIdentity(T fVal=1)
	{
	  m_aafMat[0][0] = fVal;
	  m_aafMat[0][1] = 0;
	  m_aafMat[0][2] = 0;
	  m_aafMat[0][3] = 0;
	  m_aafMat[1][0] = 0;
	  m_aafMat[1][1] = fVal;
	  m_aafMat[1][2] = 0;
	  m_aafMat[1][3] = 0;
	  m_aafMat[2][0] = 0;
	  m_aafMat[2][1] = 0;
	  m_aafMat[2][2] = fVal;
	  m_aafMat[2][3] = 0;
	  m_aafMat[3][0] = 0;
	  m_aafMat[3][1] = 0;
	  m_aafMat[3][2] = 0;
	  m_aafMat[3][3] = fVal;
	}

	//====================================================================
	//====================================================================
	void vScale(T fX,T fY,T fZ)
	{
	  m_aafMat[0][0] = fX;
	  m_aafMat[0][1] = 0;
	  m_aafMat[0][2] = 0;
	  m_aafMat[0][3] = 0;
	  m_aafMat[1][0] = 0;
	  m_aafMat[1][1] = fY;
	  m_aafMat[1][2] = 0;
	  m_aafMat[1][3] = 0;
	  m_aafMat[2][0] = 0;
	  m_aafMat[2][1] = 0;
	  m_aafMat[2][2] = fZ;
	  m_aafMat[2][3] = 0;
	  m_aafMat[3][0] = 0;
	  m_aafMat[3][1] = 0;
	  m_aafMat[3][2] = 0;
	  m_aafMat[3][3] = 1;
	}

	//====================================================================
	//====================================================================
	void vTrans(const Vector3<T> &clPt)
	{
	  vIdentity();

	  m_aafMat[0][3] = clPt.x;
	  m_aafMat[1][3] = clPt.y;
	  m_aafMat[2][3] = clPt.z;
	}

	//====================================================================
	//====================================================================
	void vRotX(T fRad)
	{
	  vIdentity();

	  m_aafMat[1][1] = (T)cos(fRad);
	  m_aafMat[1][2] = (T)sin(fRad);
	  m_aafMat[2][1] = (T)-sin(fRad);
	  m_aafMat[2][2] = (T)cos(fRad);
	}

	//====================================================================
	//====================================================================
	void vRotY(T fRad)
	{
	  vIdentity();

	  m_aafMat[0][0] = (T)cos(fRad);
	  m_aafMat[0][2] = (T)-sin(fRad);
	  m_aafMat[2][0] = (T)sin(fRad);
	  m_aafMat[2][2] = (T)cos(fRad);
	}

	//====================================================================
	//====================================================================
	void vRotZ(T fRad)
	{
	  vIdentity();

	  m_aafMat[0][0] = (T)cos(fRad);
	  m_aafMat[0][1] = (T)sin(fRad);
	  m_aafMat[1][0] = (T)-sin(fRad);
	  m_aafMat[1][1] = (T)cos(fRad);
	}

	//====================================================================
	//====================================================================
	void vFlipOrder()
	{
	  Matrix4x4<T> clMatOld = *this;

	  // matrix is in [Row][Col] format
	  for (int iRow=0; iRow<4; iRow++)
	  {
		for (int iCol=0; iCol<4; iCol++)
		{
		  m_aafMat[iRow][iCol] = clMatOld.m_aafMat[iCol][iRow];
		}
	  }
	}

	//====================================================================
	// Going in Reverse
	// Multiplying forwards through a matrix is great, but what if you want to multiply "backwards", to take a point 
	// that has been transformed through a matrix, and bring it back into the Identity reference frame where it started from? 
	// In the general case, this requires calculating the Inverse of the matrix, which is a lot of work for general matrices. 
	// However in the case of standard rotation matrices (said to represent an Orthonormal Basis) where the three Axis Vectors 
	// are at perfect right angles to each other, the Transpose of the matrix happens to also be the Inverse, and the Transpose 
	// is created merely by flipping the matrix about its primary diagonal (which runs from upper left to lower right). 
	// Here is a visual representation: 
	//
	// 
	//          [0,  0, -1]        T   [ 0, 1,  0]
	// matrix = [1,  0,  0]  matrix  = [ 0, 0, -1]
	//          [0, -1,  0]            [-1, 0,  0]
	//           ^   ^   ^  
	//           |   |   |-- Z Axis Vector
	//           |   |------ Y Axis Vector
	//           |---------- X Axis Vector
	// 
	// Effective cell order change (in RAM):
	// 
	//            [0, 3, 6]            [0, 1, 2]
	//     normal [1, 4, 7]  transpose [3, 4, 5]
	//            [2, 5, 8]            [6, 7, 8]
	// 
	// 
	// You can either make a transposed version of your matrix and then multiply by that, 
	// or you can do the math to directly multiply through the transpose of the matrix without actually changing it. 
	// If you go for the latter, it just so happens that instead of scaling and adding, you do Dot Products. 
	// The Dot Product of the vectors (X, Y, Z) and (A, B, C) is X*A + Y*B + Z*C. The following code to multiply a 
	// point through the Transpose of a matrix simply does 3 Dot Products, Point Dot X Axis Vector, Point Dot Y Axis Vector, and Point Dot Z Axis Vector. 
	//
	// Going in reverse works similarly for 3x4 matrices with Translation, except that you have to subtract the 
	// Translation Vector from the point before rotating it, to properly reverse the order of operations, since 
	// when going forwards, the Translation Vector was added to the point after rotation.
	//====================================================================
	void vReverseRotTrans(Vector3<T> &clV)
	{
	  T TX = clV.x - m_aafMat[3][0];
	  T TY = clV.y - m_aafMat[3][1];
	  T TZ = clV.z - m_aafMat[3][2];
	  clV.x = TX * m_aafMat[0][0] + TY * m_aafMat[0][1] + TZ * m_aafMat[0][2];
	  clV.y = TX * m_aafMat[1][0] + TY * m_aafMat[1][1] + TZ * m_aafMat[1][2];
	  clV.z = TX * m_aafMat[2][0] + TY * m_aafMat[2][1] + TZ * m_aafMat[2][2];

	}

	//====================================================================
	// note matrixs should be [col][row]
	//
	// Note the returned mat is a 3x4 (col)(row), so the last col will be zero
	//====================================================================
	void vMul3x4(const Vector3<T> &clRow1, const Vector3<T> &clRow2, const Vector3<T> &clRow3, const Vector3<T> &clRow4, Matrix4x4<T> *pclMat) const
	{
	  // loop though each row of the 4x4 and each row of the 
	  for (int i=0; i<4; i++)
	  {
		pclMat->m_aafMat[0][i] = m_aafMat[0][i]*clRow1.x + m_aafMat[1][i]*clRow2.x + m_aafMat[2][i]*clRow3.x + m_aafMat[3][i]*clRow4.x;
		pclMat->m_aafMat[1][i] = m_aafMat[0][i]*clRow1.y + m_aafMat[1][i]*clRow2.y + m_aafMat[2][i]*clRow3.y + m_aafMat[3][i]*clRow4.y;
		pclMat->m_aafMat[2][i] = m_aafMat[0][i]*clRow1.z + m_aafMat[1][i]*clRow2.z + m_aafMat[2][i]*clRow3.z + m_aafMat[3][i]*clRow4.z;
		pclMat->m_aafMat[3][i] = 0;
	  }
	}

	//====================================================================
	// note matrixs should be [col][row]
	//
	// This function mutiplies a 4x1 with this treated as a 3x4 which results in 3 Ts or 
	// a vector
	//====================================================================
	void vMul4x1_3x4(T *paf4, Vector3<T> *pclV) const
	{
	  pclV->x = paf4[0]*m_aafMat[0][0] + paf4[1]*m_aafMat[0][1] + paf4[2]*m_aafMat[0][2] + paf4[3]*m_aafMat[0][3];
	  pclV->y = paf4[0]*m_aafMat[1][0] + paf4[1]*m_aafMat[1][1] + paf4[2]*m_aafMat[1][2] + paf4[3]*m_aafMat[1][3];
	  pclV->z = paf4[0]*m_aafMat[2][0] + paf4[1]*m_aafMat[2][1] + paf4[2]*m_aafMat[2][2] + paf4[3]*m_aafMat[2][3];
	}

	//====================================================================
	//====================================================================
	Matrix4x4<T> operator= (const Matrix4x4<T> &clMat)
	{
	  if (this == &clMat)
		return *this;

	  m_aafMat[0][0] = clMat.m_aafMat[0][0];
	  m_aafMat[0][1] = clMat.m_aafMat[0][1];
	  m_aafMat[0][2] = clMat.m_aafMat[0][2];
	  m_aafMat[0][3] = clMat.m_aafMat[0][3];
	  m_aafMat[1][0] = clMat.m_aafMat[1][0];
	  m_aafMat[1][1] = clMat.m_aafMat[1][1];
	  m_aafMat[1][2] = clMat.m_aafMat[1][2];
	  m_aafMat[1][3] = clMat.m_aafMat[1][3];
	  m_aafMat[2][0] = clMat.m_aafMat[2][0];
	  m_aafMat[2][1] = clMat.m_aafMat[2][1];
	  m_aafMat[2][2] = clMat.m_aafMat[2][2];
	  m_aafMat[2][3] = clMat.m_aafMat[2][3];
	  m_aafMat[3][0] = clMat.m_aafMat[3][0];
	  m_aafMat[3][1] = clMat.m_aafMat[3][1];
	  m_aafMat[3][2] = clMat.m_aafMat[3][2];
	  m_aafMat[3][3] = clMat.m_aafMat[3][3];
		return *this;
	}

	//====================================================================
	//====================================================================
	Matrix4x4<T> operator= (const double ppdMat[4][4])
	{
		m_aafMat[0][0] = (T)ppdMat[0][0];
	  m_aafMat[0][1] = (T)ppdMat[0][1];
	  m_aafMat[0][2] = (T)ppdMat[0][2];
	  m_aafMat[0][3] = (T)ppdMat[0][3];
	  m_aafMat[1][0] = (T)ppdMat[1][0];
	  m_aafMat[1][1] = (T)ppdMat[1][1];
	  m_aafMat[1][2] = (T)ppdMat[1][2];
	  m_aafMat[1][3] = (T)ppdMat[1][3];
	  m_aafMat[2][0] = (T)ppdMat[2][0];
	  m_aafMat[2][1] = (T)ppdMat[2][1];
	  m_aafMat[2][2] = (T)ppdMat[2][2];
	  m_aafMat[2][3] = (T)ppdMat[2][3];
	  m_aafMat[3][0] = (T)ppdMat[3][0];
	  m_aafMat[3][1] = (T)ppdMat[3][1];
	  m_aafMat[3][2] = (T)ppdMat[3][2];
	  m_aafMat[3][3] = (T)ppdMat[3][3];
		return *this;
	}

	//====================================================================
	//====================================================================
	Matrix4x4<T> operator= (const double pdMat[16])
	{
	  m_aafMat[0][0] = (T)pdMat[0];
	  m_aafMat[0][1] = (T)pdMat[1];
	  m_aafMat[0][2] = (T)pdMat[2];
	  m_aafMat[0][3] = (T)pdMat[3];
	  m_aafMat[1][0] = (T)pdMat[4];
	  m_aafMat[1][1] = (T)pdMat[5];
	  m_aafMat[1][2] = (T)pdMat[6];
	  m_aafMat[1][3] = (T)pdMat[7];
	  m_aafMat[2][0] = (T)pdMat[8];
	  m_aafMat[2][1] = (T)pdMat[9];
	  m_aafMat[2][2] = (T)pdMat[10];
	  m_aafMat[2][3] = (T)pdMat[11];
	  m_aafMat[3][0] = (T)pdMat[12];
	  m_aafMat[3][1] = (T)pdMat[13];
	  m_aafMat[3][2] = (T)pdMat[14];
	  m_aafMat[3][3] = (T)pdMat[15];
		return *this;
	}

	//====================================================================
	//====================================================================
	Matrix4x4<T> operator* (const Matrix4x4<T> &clMat) const
	{
	  Matrix4x4<T> clNewMat;

	  // [col][row]
	  // row * col

	  //Cycle through each vector of first matrix.
	  for(int i=0; i<4; i++)
	  {  
		clNewMat.m_aafMat[i][0] = m_aafMat[i][0]*clMat.m_aafMat[0][0] + m_aafMat[i][1]*clMat.m_aafMat[1][0] + m_aafMat[i][2]*clMat.m_aafMat[2][0] + m_aafMat[i][3]*clMat.m_aafMat[3][0];
		clNewMat.m_aafMat[i][1] = m_aafMat[i][0]*clMat.m_aafMat[0][1] + m_aafMat[i][1]*clMat.m_aafMat[1][1] + m_aafMat[i][2]*clMat.m_aafMat[2][1] + m_aafMat[i][3]*clMat.m_aafMat[3][1];
		clNewMat.m_aafMat[i][2] = m_aafMat[i][0]*clMat.m_aafMat[0][2] + m_aafMat[i][1]*clMat.m_aafMat[1][2] + m_aafMat[i][2]*clMat.m_aafMat[2][2] + m_aafMat[i][3]*clMat.m_aafMat[3][2];
		clNewMat.m_aafMat[i][3] = m_aafMat[i][0]*clMat.m_aafMat[0][3] + m_aafMat[i][1]*clMat.m_aafMat[1][3] + m_aafMat[i][2]*clMat.m_aafMat[2][3] + m_aafMat[i][3]*clMat.m_aafMat[3][3];
	  }

	  return clNewMat;
	}

	//====================================================================
	//====================================================================
	/*
	Matrix4x4<T> operator* (const CMatrix3x3 &clMat) const
	{
	  Matrix4x4<T> clNewMat;
		//Cycle through each vector of first matrix.
	  for(int i=0; i<3; i++)
	  {  
		clNewMat.m_aafMat[i][0] = m_aafMat[i][0]*clMat.m_aafMat[0][0] + m_aafMat[i][1]*clMat.m_aafMat[1][0] + m_aafMat[i][2]*clMat.m_aafMat[2][0];
		clNewMat.m_aafMat[i][1] = m_aafMat[i][0]*clMat.m_aafMat[0][1] + m_aafMat[i][1]*clMat.m_aafMat[1][1] + m_aafMat[i][2]*clMat.m_aafMat[2][1];
		clNewMat.m_aafMat[i][2] = m_aafMat[i][0]*clMat.m_aafMat[0][2] + m_aafMat[i][1]*clMat.m_aafMat[1][2] + m_aafMat[i][2]*clMat.m_aafMat[2][2];
		clNewMat.m_aafMat[i][3] = m_aafMat[i][3]; // assume identity on the 3x3 so just set the last value
	  }

	  return clNewMat;
	}
	*/

	//====================================================================
	//====================================================================
	Matrix4x4<T> operator* (T f) const
	{
	  Matrix4x4<T> clNewMat;

	  for (int iCol=0; iCol<4; iCol++)
	  {
		for (int iRow=0; iRow<4; iRow++)
		{
		  clNewMat.m_aafMat[iCol][iRow] = m_aafMat[iCol][iRow]*f;
		}
	  }

	  return clNewMat;
	}

	//====================================================================
	//====================================================================
	Vector3<T> operator* (const Vector3<T> &clV) const
	{
	  Vector3<T> clNewV;

	  clNewV.x = m_aafMat[0][0]*clV.x + m_aafMat[0][1]*clV.y + m_aafMat[0][2]*clV.z + m_aafMat[0][3]; // (vector w coord is 1)
	  clNewV.y = m_aafMat[1][0]*clV.x + m_aafMat[1][1]*clV.y + m_aafMat[1][2]*clV.z + m_aafMat[1][3]; // (vector w coord is 1)
	  clNewV.z = m_aafMat[2][0]*clV.x + m_aafMat[2][1]*clV.y + m_aafMat[2][2]*clV.z + m_aafMat[2][3]; // (vector w coord is 1)
	  return clNewV;
	}

	//====================================================================
	//====================================================================
	Matrix4x4<T>& operator*= (const Matrix4x4<T> &clMat)
	{
	  *this = *this * clMat;

	  return *this;
	}

	//====================================================================
	// multiply this by a scalar
	//====================================================================
	Matrix4x4<T>& operator*= (const T f)
	{
	  for (int iCol=0; iCol<4; iCol++)
	  {
		for (int iRow=0; iRow<4; iRow++)
		{
		  m_aafMat[iCol][iRow] = m_aafMat[iCol][iRow]*f;
		}
	  }

	  return *this;
	}

};


typedef Matrix4x4<float> mat4x4f;
/*
class Matrix4x4
{
  // methods
  public:
    Matrix4x4();
    Matrix4x4(const Matrix4x4 &clMat);
    virtual ~Matrix4x4();

    void vGet(T *paf16) const;
    void vSet(T *paf16);

    void vIdentity(T fVal=1.0f);

    void vScale(T fX=1,T fY=1,T fZ=1);
    void vTrans(const vec3f &clPt);
    void vRotX(T fRad);
    void vRotY(T fRad);
    void vRotZ(T fRad);

    void vFlipOrder();

    void vReverseRotTrans(vec3f &clV);

    void vMul3x4(const vec3f &clRow1, const vec3f &clRow2, const vec3f &clRow3, const vec3f &clRow4, Matrix4x4 *pclMat3x4) const;
    void vMul4x1_3x4(T *paf4, vec3f *pclV) const;

    Matrix4x4 operator= (const Matrix4x4 &clMat);
    Matrix4x4 operator= (const double ppdMat[4][4]);
    Matrix4x4 operator= (const double pdMat[16]);
    Matrix4x4 operator* (const Matrix4x4 &clMat) const;
    Matrix4x4 operator* (const Matrix3x3 &clMat) const;
    Matrix4x4 operator* (T f) const;	            // mutiply by a scalar	
    vec3f operator* (const vec3f &clV) const;	    // mutiply by a vector	

    Matrix4x4& operator*= (const Matrix4x4 &clMat);
    Matrix4x4& operator*= (const T f);     	   // mutiply by a scalar

  // data members
  public:
    T m_aafMat[4][4]; // note matrix should be [col][row] for opengl, so this matrix is [col][row]
};
*/

#endif
