#include "vaser.h"
#include "vaser_gradient.h"
#include "vaser_polyline.h"
#include "vaser_point.h"
#include "vaser_agg_curve4.h"
#include <vector>

namespace VASEr
{
	namespace VASErin
	{	//VASEr internal namespace

		class polyline_buffer
		{
		public:
			std::vector<Vec2>   P;
			std::vector<Color>  C;
			std::vector<real> W;
			std::vector<real> L; //length along polyline
			int N;
			real path_length; //total segment length

			polyline_buffer() : P(), C(), W(), L()
			{
				N = 0;
				path_length = 0.0;
				L.push_back(0.0);
			}
			void point(real x, real y)
			{
				Vec2 V = { x, y };
				addvertex(V);
			}
			void point(Vec2 V)
			{
				addvertex(V);
			}
			static void point_cb(void* obj, real x, real y)
			{
				polyline_buffer* This = (polyline_buffer*)obj;
				Vec2 V = { x, y };
				This->addvertex(V);
			}
			void vertex(Vec2 V, Color cc)
			{
				addvertex(V, &cc);
			}
			void vertex(Vec2 V, Color cc, real ww)
			{
				addvertex(V, &cc, ww);
			}
			void set_color(Color cc)
			{
				if (!C.size())
					C.push_back(cc);
				else
					C.back() = cc;
			}
			void set_weight(real ww)
			{
				if (!W.size())
					W.push_back(ww);
				else
					W.back() = ww;
			}
			void gradient(const gradient* grad)
			{
				gradient_apply(grad, &C[0], &W[0], &L[0], N, path_length);
			}
			void draw(const polyline_opt* options)
			{
				if (!N) return;
				polyline_inopt in_options = { 0 };
				in_options.segment_length = &L[0];
				polyline(&P[0], &C[0], &W[0], N, options, &in_options);
			}
		private:
			bool addvertex(const Vec2& V, const Color* cc = 0, real ww = 0.0)
			{
				if (N && P[N - 1].x == V.x && P[N - 1].y == V.y)
					return false; //duplicate
				else
				{
					//point
					P.push_back(V);
					if (N > 0)
					{
						real len = (Point(V) - Point(P[N - 1])).length();
						path_length += len;
						L.push_back(len);
					}
					//color
					if (cc)
						C.push_back(*cc);
					else
					{
						if (!C.size())
							C.push_back(default_color);
						else
							C.push_back(C.back());
					}
					//weight
					if (ww)
						W.push_back(ww);
					else
					{
						if (!W.size())
							W.push_back(default_weight);
						else
							W.push_back(W.back());
					}
					//finally
					N++;
					return true;
				}
			}
		};

		struct polybezier_inopt
		{
			bool evaluate_only;
			polyline_buffer* target;
		};

		void polybezier(const Vec2* P, const gradient* grad, int length, const polybezier_opt* options, const polybezier_inopt* in_options)
		{
			polybezier_opt opt = { 0 };
			polybezier_inopt inopt = { 0 };
			polyline_opt poly = { 0 };
			poly.joint = PLJ_bevel;
			if (options) opt = *options;
			if (in_options) inopt = *in_options;
			if (!opt.poly) opt.poly = &poly;

			polyline_buffer _buf;
			polyline_buffer& buf = _buf;
			if (inopt.target)
				buf = *inopt.target;

			const real BZ_default_approximation_scale = 0.5;
			const real BZ_default_angle_tolerance = 15.0 / 180 * vaser_pi;
			const real BZ_default_cusp_limit = 5.0;

			for (int i = 0; i < length - 3; i += 3)
			{
				curve4_div(
					P[i + 0].x, P[i + 0].y,
					P[i + 1].x, P[i + 1].y,
					P[i + 2].x, P[i + 2].y,
					P[i + 3].x, P[i + 3].y,
					BZ_default_approximation_scale,
					BZ_default_angle_tolerance,
					BZ_default_cusp_limit,
					polyline_buffer::point_cb,
					&buf);
			}
			if (grad)
				buf.gradient(grad);
			if (!inopt.evaluate_only)
				buf.draw(opt.poly);
		}

	} //sub namespace VASErin
}

//export implementations
namespace VASEr
{

	void polybezier(const Vec2* P, const gradient* grad, int length, const polybezier_opt* options)
	{
		VASErin::polybezier(P, grad, length, options, 0);
	}

	void polybezier(const Vec2* P, Color cc, real ww, int length, const polybezier_opt* options)
	{
		gradient grad = { 0 };
		gradient_stop stop[2] = { 0 };
		grad.stops = stop;
		grad.length = 2;
		stop[0].type = GS_rgba; stop[0].color = cc;
		stop[1].type = GS_weight; stop[1].weight = ww;
		VASErin::polybezier(P, &grad, length, options, 0);
	}
}
