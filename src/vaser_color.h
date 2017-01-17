#pragma once
#include "vaser.h"
#include <math.h>

namespace VASEr
{
	namespace VASErin
	{

		float& Color_get(Color& C, int index);
		bool Color_valid_range(float t);
		Color Color_between(const Color& A, const Color& B, float t = 0.5f);
		void sRGBtolinear(Color& C, bool exact = false);
		void lineartosRGB(Color& C, bool exact = false);
		float color_max(float r, float g, float b);
		float color_min(float r, float g, float b);
		void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v);
		void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v);
	}
}
