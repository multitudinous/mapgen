#pragma once

#include "vaser_color.h"

namespace VASEr
{
	namespace VASErin
	{	//VASEr internal namespace

		real grad_getstep(real A, real B, real t, real T);

		void gradient_apply(const gradient* gradp, Color* C, real* W, const real* L, int length, real path_length);

	} //sub namespace VASErin
}
