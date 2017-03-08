#pragma once
#include "vaser.h"

namespace VASEr
{
	namespace VASErin
	{	//VASEr internal namespace

		struct polyline_inopt
		{
			bool const_color;
			bool const_weight;
			bool no_cap_first;
			bool no_cap_last;
			bool join_first;
			bool join_last;
			real* segment_length; //array of length of each segment
		};


		void polyline(
			const Vec2* PP,  //pointer to array of point of a polyline
			const Color* C,  //array of color
			const real* W, //array of weight
			int length, //size of the buffer P
			const polyline_opt* options, //options
			const polyline_inopt* in_options); //internal options
		
	}
}