#ifndef VASER_VERTEX_ARRAY_HOLDER_H
#define VASER_VERTEX_ARRAY_HOLDER_H

#include "vaser.h"
#include "vaser_point.h"

#include <vector>

namespace VASEr
{
	namespace VASErin
	{
	class vertex_array_holder
	{
	public:
		int count; //counter
		int glmode; //drawing mode in opengl
		bool jumping;
		std::vector<float> vert; //because it holds 2d vectors
		std::vector<float> color; //RGBA

		vertex_array_holder();

		void set_gl_draw_mode(int gl_draw_mode);

		void clear();
		void move(int a, int b);
		
		void replace(int a, Point P, Color C);

		int push(const Point& P, const Color& cc, bool trans = false);
		void push3(const Point& P1, const Point& P2, const Point& P3,
			const Color& C1, const Color& C2, const Color& C3,
			bool trans1 = 0, bool trans2 = 0, bool trans3 = 0);

		void push(const vertex_array_holder& hold);

		Point get(int i);
		Color get_color(int b);
		Point get_relative_end(int di = -1);
		void repeat_last_push();
		void jump();
		
		void draw();
		
		void draw_triangles();

		void swap(vertex_array_holder& B);
	};

}
}
#endif
