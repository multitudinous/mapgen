#pragma once

#include "vaser.h"

namespace VASEr
{
	struct polyline_opt;

	namespace VASErin
	{

		class vertex_array_holder;

		class backend
		{
		public:
			static void vah_draw(vertex_array_holder& vah);
			static void polyline(const Vec2*, Color, double W, int length, const polyline_opt*); //constant color and weight
		};
	}
}