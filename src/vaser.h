#ifndef VASER_H
#define VASER_H

#include "sysdefs.h"

/* Vase Renderer first draft, version 0.3 */

/* Basic usage
* You should provide these structs before any vase_renderer include, using
struct Vec2 { real x,y;};
struct Color { float r,g,b,a;};
* or
typedef your_vec2 Vec2;
typedef your_color Color;
*/

//#define real double
#define real float

struct Vec2 { real x, y; };
struct Color { float r, g, b, a; };

#define VASER_DEBUG 1

#ifdef VASER_DEBUG
#include "logger.h"
#define DEBUG LogTrace
#else
#define DEBUG ;//
#endif


namespace VASEr
{
const real vaser_pi=3.141592653589793;

struct gradient_stop
{
	real t; //position
	char type; //GS_xx
	union
	{
		Color color;
		real weight;
	};
};
	const char GS_none  =0;
	const char GS_rgba  =1;
	const char GS_rgb   =2; //rgb only
	const char GS_alpha =3;
	const char GS_weight=4;
struct gradient
{
	gradient_stop* stops; //array must be sorted in ascending order of t
	int length; //number of stops
	char unit; //use_GD_XX
};
	const char GD_ratio =0; //default
	const char GD_length=1;

struct Image
{
	unsigned char* buf; //must **free** buffer manually
	short width;
	short height;
};

class renderer
{
public:
	static void init();
	static void before();
	static void after();
	static Image get_image();
};

struct tessellator_opt
{
	//set the whole structure to 0 will give default options
	bool triangulation;
	char parts; //use TS_xx
	bool tessellate_only;
	void* holder; //used as (VASErin::vertex_array_holder*) if tessellate_only is true
};
	//for tessellator_opt.parts
	const char TS_core_fade =0; //default
	const char TS_core      =1;
	const char TS_outer_fade=2;
	const char TS_inner_fade=3;

struct polyline_opt
{
	//set the whole structure to 0 will give default options
	const tessellator_opt* tess;
	char joint; //use PLJ_xx
	char cap;   //use PLC_xx
	bool feather;
		real feathering;
		bool no_feather_at_cap;
		bool no_feather_at_core;
};
	//for polyline_opt.joint
	const char PLJ_miter =0; //default
	const char PLJ_bevel =1;
	const char PLJ_round =2;
	//for polyline_opt.cap
	const char PLC_butt  =0; //default
	const char PLC_round =1;
	const char PLC_square=2;
	const char PLC_rect  =3;
	const char PLC_both  =0; //default
	const char PLC_first =10;
	const char PLC_last  =20;
	const char PLC_none  =30;

void polyline( const Vec2*, const Color*, const real*, int length, const polyline_opt*);
void polyline( const Vec2*, Color, real W, int length, const polyline_opt*); //constant color and weight
void polyline( const Vec2*, const Color*, real W, int length, const polyline_opt*); //constant weight
void polyline( const Vec2*, Color, const real* W, int length, const polyline_opt*); //constant color
void segment( Vec2, Vec2, Color, Color, real W1, real W2, const polyline_opt*);
void segment( Vec2, Vec2, Color, real W, const polyline_opt*); //constant color and weight
void segment( Vec2, Vec2, Color, Color, real W, const polyline_opt*); //constant weight
void segment( Vec2, Vec2, Color, real W1, real W2, const polyline_opt*); //const color

struct polybezier_opt
{
	//set the whole structure to 0 will give default options
	const polyline_opt* poly;
};

void polybezier( const Vec2*, const gradient*, int length, const polybezier_opt*);
void polybezier( const Vec2*, Color, real W, int length, const polybezier_opt*);

namespace VASErin
{ //VASEr internal namespace
	const real vaser_min_alw = 0.00000000001; //smallest value not regarded as zero
	const Color default_color = { 0, 0, 0, 1 };
	const real default_weight = 1.0;
}

} //namespace VASEr

#endif
