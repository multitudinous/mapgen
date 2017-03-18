#include "vaser_backend.h"
#include "vaser_vertex_array_holder.h"

namespace VASEr
{
	namespace VASErin
	{	//VASEr internal namespace

		void backend::vah_draw(vertex_array_holder& vah)
		{
			if (vah.count > 0) //save some effort
			{
				static size_t count = 0;
				count++;

				if (count > 818)
				{
					return;
				}

				int type = GL_FLOAT;
				if (sizeof(real) == 8) type = GL_DOUBLE;
				
				
				glVertexPointer(2, type, 0, &vah.vert[0]);
				glColorPointer(4, GL_FLOAT, 0, &vah.color[0]);
				glDrawArrays(vah.glmode, 0, vah.count);
				

				
				DEBUG("draw count %d", count);

				/*
				for (size_t i = 0; i < vah.vert.size(); i += 2)
				{
					DEBUG("vaser pt %lf, %lf", vah.vert[i], vah.vert[i + 1]);
				}
				*/
				
			}
		}

		void backend::polyline(const Vec2* P, Color C, real W, int length, const polyline_opt*) //constant color and weight
		{
			int type = 0;
			if (sizeof(Vec2) == 16)
				type = GL_DOUBLE;
			else if (sizeof(Vec2) == 8)
				type = GL_FLOAT;

			glColor4f(C.r, C.g, C.b, C.a);
			glLineWidth(W);
			if (type)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glVertexPointer(2, type, 0, P);
				glDrawArrays(GL_LINE_STRIP, 0, length);
				glEnableClientState(GL_COLOR_ARRAY);
			}
			else
			{
				glBegin(GL_LINE_STRIP);
				for (int i = 0; i < length; i++)
					glVertex2f(P[i].x, P[i].y);
				glEnd();
			}
			glLineWidth(1);
		}
	}
}