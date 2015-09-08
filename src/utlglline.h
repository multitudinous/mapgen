#pragma once

#include "point2d.h"
#include "vbo.h"
#include "rgbf.h"
#include "shaderprog.h"
class UtlGlLine
{
public:
    enum line_cap_e
    {
        square_cap = 0,
        butt_cap = 1,
        round_cap = 2
    };

    enum line_join_e
    {
        bevel_join = 0,
        miter_join = 1,
        round_join = 2
    };

public:

    static PShaderProg initSmoothShader();
    static PVbo createLine(const ListPt2d &points, float thickness, int join, int cap, float alpha=1);
    static PVbo createLined(const ListPt2d &points, float thickness, int join, int cap, float alpha=1);
};

