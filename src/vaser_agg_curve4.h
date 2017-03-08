#pragma once
#include "vaser.h"

namespace VASEr
{
	namespace VASErin
	{	//VASEr internal namespace

		int curve4_div(real x1, real y1,
			real x2, real y2,
			real x3, real y3,
			real x4, real y4,
			real m_approximation_scale,
			real m_angle_tolerance,
			real m_cusp_limit,
			void(*add_point)(void*, real, real),
			void* obj);

	}
}