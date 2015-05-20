#ifndef RGBF_H
#define RGBF_H

#include "sysdefs.h"

class Rgbf
{
    public:
        enum
        {
            R = 0,
            G = 1,
            B = 2,
            A = 3
        };

    // methods
    public:
        Rgbf(float  fR=0, float fG=0, float fB=0, float fA=1);
        Rgbf(int iR, int iG, int iB, int iA=1);
        Rgbf(const Rgbf &clRGB);
        Rgbf(float fLevel);
        //Rgbf(DWORD dwRGB);
        ~Rgbf();

        void SetColor(float r, float g, float b, float a);
        void SetColor(float r, float g, float b);
        void SetColor(BYTE *pPixel, int BytesPerPixel=4, bool bgr=false);
        void SetColorR(float r);
        void SetColorG(float g);
        void SetColorB(float b);
        void SetColorA(float a);

        float GetR() const { return m_af[R]; }
        float GetG() const { return m_af[G]; }
        float GetB() const { return m_af[B]; }
        float GetA() const { return m_af[A]; }
        BYTE GetRB() const { return (BYTE)(255.0*m_af[R]); }
        BYTE GetGB() const { return (BYTE)(255.0*m_af[G]); }
        BYTE GetBB() const { return (BYTE)(255.0*m_af[B]); }
        BYTE GetAB() const { return (BYTE)(255.0*m_af[A]); }
        const float* GetColor() const { return m_af;  }
        


        void vScale();

        Rgbf& operator= (const Rgbf &clRGB);
        //Rgbf& operator= (DWORD dwRGB);
        Rgbf& operator= (float fLevel);
        Rgbf  operator+ (const Rgbf &clRGB);
        Rgbf  operator- (const Rgbf &clRGB);
        Rgbf  operator* (float fLevel);
        Rgbf  operator* (const Rgbf &clRGB);
        Rgbf& operator+= (float fLevel);
        Rgbf& operator+= (const Rgbf &clRGB);
        Rgbf& operator-= (float fLevel);
        Rgbf& operator-= (const Rgbf &clRGB);
        Rgbf& operator/= (float fLevel);
        Rgbf& operator*= (float fLevel);
        Rgbf& operator*= (const Rgbf &clRGB);
        Rgbf& operator++ ();
        Rgbf& operator-- ();
        //operator DWORD() const;
        float& operator[]		 (int i);
        float  operator[]		 (int i) const;

        friend Rgbf operator * ( float r, const Rgbf& v ) { return Rgbf( v[0] * r, v[1] * r, v[2] * r, v[3] ); }
        friend Rgbf operator * ( const Rgbf& v, float r) { return Rgbf( v[0] * r, v[1] * r, v[2] * r, v[3] ); }


    // data members
    public:
        float m_af[4]; // rgba
};

#endif
