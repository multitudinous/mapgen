#pragma once

namespace VASEr
{
	namespace VASErin
	{	//VASEr internal namespace

		int curve4_div(double x1, double y1,
			double x2, double y2,
			double x3, double y3,
			double x4, double y4,
			double m_approximation_scale,
			double m_angle_tolerance,
			double m_cusp_limit,
			void(*add_point)(void*, double, double),
			void* obj);

	}
}