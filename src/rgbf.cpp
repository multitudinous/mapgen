#include "rgbf.h"
#include <assert.h>
#include <cstring>

//=======================================================================
//=======================================================================
Rgbf::Rgbf(float fR, float fG, float fB, float fA)
{
    m_af[R] = fR;
    m_af[G] = fG;
    m_af[B] = fB;
    m_af[A] = fA;
}

Rgbf::Rgbf(int iRl, int iGl, int iBl, int iAl)
{
    m_af[R] = (float)iRl;
    m_af[G] = (float)iGl;
    m_af[B] = (float)iBl;
    m_af[A] = (float)iAl;

    /*
    m_af[R] = (float)iRl/255.0;
    m_af[G] = (float)iGl/255.0;
    m_af[B] = (float)iBl/255.0;
    m_af[A] = (float)iAl/255.0;
    */
}

Rgbf::Rgbf(const Rgbf &clRGB)
{
    memcpy(m_af, clRGB.m_af, sizeof(float)*4);
}

Rgbf::Rgbf(float fLevel)
{
    m_af[R] = fLevel;
    m_af[G] = fLevel;
    m_af[B] = fLevel;
    m_af[A] = 1;
}

/*
Rgbf::Rgbf(DWORD dwRGB)
{
    m_af[R] = (float)GetRValue(dwRGB)/255.0f;
    m_af[G] = (float)GetGValue(dwRGB)/255.0f;
    m_af[B] = (float)GetBValue(dwRGB)/255.0f;
    m_af[A] = 1;
}
*/

Rgbf::~Rgbf()
{
}

void Rgbf::SetColor(float r, float g, float b, float a)
{
    m_af[R] = r;
    m_af[G] = g;
    m_af[B] = b;
    m_af[A] = a;
}

void Rgbf::SetColor(float r, float g, float b)
{
    m_af[R] = r;
    m_af[G] = g;
    m_af[B] = b;
}

void Rgbf::SetColor(BYTE *pPixel, int BytesPerPixel, bool bgr)
{
    if (BytesPerPixel == 4)
        m_af[A] = (float)pPixel[3]/255.0f;
    if (BytesPerPixel >= 3)
    {
        if (bgr)
        {
            m_af[B] = (float)pPixel[0]/255.0f;
            m_af[G] = (float)pPixel[1]/255.0f;
            m_af[R] = (float)pPixel[2]/255.0f;
        }
        else
        {
            m_af[R] = (float)pPixel[0]/255.0f;
            m_af[G] = (float)pPixel[1]/255.0f;
            m_af[B] = (float)pPixel[2]/255.0f;
        }
    }
}

void Rgbf::SetColorR(float r)
{
    m_af[R] = r;
}

void Rgbf::SetColorG(float g)
{
    m_af[G] = g;
}

void Rgbf::SetColorB(float b)
{
    m_af[B] = b;
}

void Rgbf::SetColorA(float a)
{
    m_af[A] = a;
}

void Rgbf::vScale()
{
    for (int i=0; i<4; i++)
    {
        if (m_af[i] > 1) m_af[i] = 1;
        else if (m_af[i]< 0) m_af[i] = 0;
    }
}

Rgbf& Rgbf::operator= (const Rgbf &clRGB)
{
     if (this == &clRGB)
    return *this;

    memcpy(m_af, clRGB.m_af, sizeof(float)*4);

    return *this;
}

/*
Rgbf& Rgbf::operator= (DWORD dwRGB)
{
    m_af[R] = (float)GetRValue(dwRGB)/255.0f;
    m_af[G] = (float)GetGValue(dwRGB)/255.0f;
    m_af[B] = (float)GetBValue(dwRGB)/255.0f;
    // TODO: alpha

    return *this;
}
*/

Rgbf& Rgbf::operator= (float fLevel)
{
    m_af[R] = fLevel;
    m_af[G] = fLevel;
    m_af[B] = fLevel;

    return *this;
}

Rgbf Rgbf::operator+ (const Rgbf &clRGB)
{
    Rgbf clRGBN;
    clRGBN.m_af[R] = m_af[R]+clRGB.m_af[R];
    clRGBN.m_af[G] = m_af[G]+clRGB.m_af[G];
    clRGBN.m_af[B] = m_af[B]+clRGB.m_af[B];

    return clRGBN;
}

Rgbf Rgbf::operator- (const Rgbf &clRGB)
{
    Rgbf clRGBN;
    clRGBN.m_af[R] = m_af[R]-clRGB.m_af[R];
    clRGBN.m_af[G] = m_af[G]-clRGB.m_af[G];
    clRGBN.m_af[B] = m_af[B]-clRGB.m_af[B];

    return clRGBN;
}

Rgbf Rgbf::operator* (float fLevel)
{
    Rgbf clRGB;
    clRGB.m_af[R] = m_af[R]*fLevel;
    clRGB.m_af[G] = m_af[G]*fLevel;
    clRGB.m_af[B] = m_af[B]*fLevel;

    return clRGB;
}

Rgbf  Rgbf::operator* (const Rgbf &clRGB)
{
    Rgbf clRGBN;
    clRGBN.m_af[R] = m_af[R]*clRGB.m_af[R];
    clRGBN.m_af[G] = m_af[G]*clRGB.m_af[G];
    clRGBN.m_af[B] = m_af[B]*clRGB.m_af[B];

    return clRGBN;
}

Rgbf& Rgbf::operator+= (float fLevel)
{
    m_af[R] += fLevel;
    m_af[G] += fLevel;
    m_af[B] += fLevel;

    return *this;
}

Rgbf& Rgbf::operator+= (const Rgbf &clRGB)
{
    m_af[R] += clRGB.m_af[R];
    m_af[G] += clRGB.m_af[G];
    m_af[B] += clRGB.m_af[B];

    return *this;
}

// +
// *****************************************************************************
//   Module      :  Rgbf& Rgbf::operator-= (int iLevel)
//   Access      :  Public
//   Input       :  iLevel - the level to sub by
//   Output      :  None
//   Return Val  :  the updated rgb class
//   Exceptions  :  None
//   Desc        :  This function overloads the -=operator.
//
//   Notes       :  None
// *****************************************************************************
// -
Rgbf& Rgbf::operator-= (float fLevel)
{
    m_af[R] -= fLevel;
    m_af[G] -= fLevel;
    m_af[B] -= fLevel;

    return *this;
}

// +
// *****************************************************************************
//   Module      :  Rgbf& Rgbf::operator-= (const Rgbf &clRGB)
//   Access      :  Public
//   Input       :  clRGB - the rgb to sub by
//   Output      :  None
//   Return Val  :  the updated rgb class
//   Exceptions  :  None
//   Desc        :  This function overloads the -=operator.
//
//   Notes       :  None
// *****************************************************************************
// -
Rgbf& Rgbf::operator-= (const Rgbf &clRGB)
{
    m_af[R] -= clRGB.m_af[R];
    m_af[G] -= clRGB.m_af[G];
    m_af[B] -= clRGB.m_af[B];

    return *this;
}

Rgbf& Rgbf::operator/= (float fLevel)
{
    m_af[R] /= fLevel;
    m_af[G] /= fLevel;
    m_af[B] /= fLevel;

    return *this;
}

Rgbf& Rgbf::operator*= (float fLevel)
{
    m_af[R] *= fLevel;
    m_af[G] *= fLevel;
    m_af[B] *= fLevel;

    return *this;
}

Rgbf& Rgbf::operator*= (const Rgbf &clRGB)
{
    for (int i=0; i<4; i++)
    {
        m_af[i] *= clRGB[i];
    }

    return *this;
}

// +
// *****************************************************************************
//   Module      :  Rgbf& Rgbf::operator++ ()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  the updated rgb class
//   Exceptions  :  None
//   Desc        :  This function overloads the ++operator.
//
//   Notes       :  None
// *****************************************************************************
// -
Rgbf& Rgbf::operator++ ()
{
    float f = 1.0f/255.0f;
    m_af[R] += f;
    m_af[G] += f;
    m_af[B] += f;

    return *this;
}

// +
// *****************************************************************************
//   Module      :  Rgbf& Rgbf::operator-- ()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  the updated rgb class
//   Exceptions  :  None
//   Desc        :  This function overloads the --operator.
//
//   Notes       :  None
// *****************************************************************************
// -
Rgbf& Rgbf::operator-- ()
{
    float fDec = 1.0f/255.0f;
    m_af[R] -= fDec;
    m_af[G] -= fDec;
    m_af[B] -= fDec;

    return *this;
}

/*
Rgbf::operator DWORD() const
{
    int iR = (int)(m_af[R] * 255.0);
    int iG = (int)(m_af[G] * 255.0);
    int iB = (int)(m_af[B] * 255.0);

    return Rgbf(iR,iG,iB);
}
*/

//=======================================================================
//=======================================================================
float& Rgbf::operator[](int i)
{
    assert(static_cast<unsigned int>(i) < 4);	// Only 3 elements to access
    return m_af[i];
}

//=======================================================================
//=======================================================================
float Rgbf::operator[](int i) const
{
    assert(static_cast<unsigned int>(i) < 4);	// Only 3 elements to access
    return m_af[i];
}
