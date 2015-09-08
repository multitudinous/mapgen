#include "utlglline.h"

//============================================================================
//============================================================================
PShaderProg UtlGlLine::initSmoothShader()
{
    PShaderProg prog(new ShaderProg());

    const char *vert = R"(uniform vec4 ucolor;
                            void main (void)
                            {
                                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                                gl_TexCoord[0].xyz = gl_MultiTexCoord0.xyz;
                                gl_FrontColor = vec4(ucolor.rgb, gl_Color.a);
                            })";

    const char *frag = R"(void main(void)
                            {
                                vec4 color = gl_Color;
                                vec3  p = gl_TexCoord[0].xyz;
                                float dist;

                                if( p.z < 0.0 )
                                {
                                    float xdist = min(abs(p.x),abs(p.x-1.0));
                                    float xwidth = fwidth(xdist);
                                    float x = smoothstep(1.0-xwidth, 1.0+xwidth, xdist);

                                    float ydist = abs(p.y);
                                    float ywidth = fwidth(ydist);
                                    float y = smoothstep(1.0-ywidth, 1.0+ywidth, ydist);

                                    float alpha = 1.0-max(x,y);
                                    gl_FragColor = vec4(color.rgb, color.a*alpha);
                                    return;
                                }

                                if( (p.x < 0.0) )
                                    dist = sqrt(p.x*p.x+p.y*p.y);
                                else if( (p.x > 1.0) )
                                    dist = sqrt((p.x-1.0)*(p.x-1.0)+p.y*p.y);
                                else
                                    dist = abs(p.y);
                                float width = fwidth(dist);
                                if( abs(p.z) <= 1.0) // alpha coverage
                                    width = dist;
                                float alpha = smoothstep(1.0+width,1.0-width, dist);

                                gl_FragColor = vec4(color.rgb, color.a*alpha);
                            })";

    try
    {
        prog->loadVertStr(vert);
        prog->loadFragStr(frag);
        return prog;
    }
    catch (std::exception e)
    {
        LogError("UtlGlLine::initSmoothShader() - shader load failed: %s", e.what());
        return PShaderProg();
    }
}

//============================================================================
//============================================================================
PVbo UtlGlLine::createLine(const ListPt2d &points, float thickness, int join, int cap, float alpha)
{
    typedef struct { float x, y, z, r, g, b, a, s, t, u; } vertex_t;

    int n_points = (int)points.size();
    float d, w;

    float r = 1;
    float g = 1;
    float b = 1;
    float a = alpha;
    float t = thickness;

    if (thickness < 1.0)
    {
        w = 2.0;
        a *= thickness;
        d = (w + 2) / w;
    }
    else
    {
        d = (thickness + 2.0) / thickness;
        w = thickness + 2.0;
    }
    

    size_t n_vertices = 0;
    size_t n_indices = 0;
    if ((join == bevel_join))
    {
        n_vertices = 2 + (n_points - 2) * 6 + 4;
        n_indices = 2 * 3 + (n_points - 2) * 3 * 3 + 6;
    }
    else if (join == miter_join)
    {
        n_vertices = 2 + (n_points - 2) * 6 + 4;
        n_indices = 2 * 3 + (n_points - 2) * 4 * 3 + 6;
    }
    else
    {
        n_vertices = 2 + (n_points - 2) * 10 + 4;
        n_indices = 2 * 3 + (n_points - 2) * 6 * 3 + 6;
    }
    vertex_t * vertices = (vertex_t *)calloc(n_vertices, sizeof(vertex_t));
    GLuint * indices = (GLuint *)calloc(n_indices, sizeof(GLuint));

    Point2d A, B, C;
    Point2d A1, A2, C1, C2;
    Point2d T_ba, O_ab, T_bc, O_cb;
    Point2d I, J, K, L, T;
    GLuint i;
    GLuint v_index = 0;
    GLuint v_count = 0;
    GLuint i_index = 0;
    GLuint i_count = 0;
    for (i = 0; i< (n_points - 2); ++i)
    {
        v_count = 0;
        i_count = 0;

        A = points[i + 0];
        B = points[i + 1];
        C = points[i + 2];
        T_ba = Point2d( B.x() - A.x(), B.y() - A.y() );
        T_bc = Point2d( B.x() - C.x(), B.y() - C.y() );
        O_ab = Point2d::ortho(A, B);
        O_cb = Point2d::ortho(C, B);
        C1 = Point2d( C.x() - w / 2 * O_cb.x(), C.y() - w / 2 * O_cb.y() );
        C2 = Point2d( C.x() + w / 2 * O_cb.x(), C.y() + w / 2 * O_cb.y() );
        if (i == 0)
        {
            A1 = Point2d( A.x() + w / 2 * O_ab.x(), A.y() + w / 2 * O_ab.y() );
            A2 = Point2d( A.x() - w / 2 * O_ab.x(), A.y() - w / 2 * O_ab.y());
        }

        Point2d::intersection(A1, Point2d(A1.x() + T_ba.x(), A1.y() + T_ba.y()), C1, Point2d(C1.x() + T_bc.x(), C1.y() + T_bc.y()), &I);
        int cross = Point2d::intersection(A2, Point2d(A2.x() + T_ba.x(), A2.y() + T_ba.y()), C2, Point2d(C2.x() + T_bc.x(), C2.y() + T_bc.y()), &L);

        if (cross == 2)
        {
            J = Point2d( B.x() + w / 2 * O_ab.x(), B.y() + w / 2 * O_ab.y() );
            K = Point2d( B.x() - w / 2 * O_cb.x(), B.y() - w / 2 * O_cb.y() );
        }
        else
        {
            T = I; I = L; L = T;
            T = A1; A1 = A2; A2 = T;
            T = C1; C1 = C2; C2 = T;
            J = Point2d( B.x() - w / 2 * O_ab.x(), B.y() - w / 2 * O_ab.y() );
            K = Point2d( B.x() + w / 2 * O_cb.x(), B.y() + w / 2 * O_cb.y() );
        }

        // Cap start
        if (i == 0)
        {
            Point2d T = Point2d::tangent(B, A);
            Point2d cA1 = Point2d(A1.x() + w / 2 * T.x(), A1.y() + w / 2 * T.y());
            Point2d cA2 = Point2d(A2.x() + w / 2 * T.x(), A2.y() + w / 2 * T.y());

            float tt = t;
            if ((cap == square_cap) || (cap == butt_cap))
            {
                tt = -t;
            }

            vertices[v_index + 0] = vertex_t{ cA1.x(), cA1.y(), 0, r, g, b, a, -d, +d, tt }; // cA1
            vertices[v_index + 1] = vertex_t{ cA2.x(), cA2.y(), 0, r, g, b, a, -d, -d, tt }; // cA2
            vertices[v_index + 2] = vertex_t{ A1.x(), A1.y(), 0, r, g, b, a, 0, +d, tt }; // A1
            vertices[v_index + 3] = vertex_t{ A2.x(), A2.y(), 0, r, g, b, a, 0, -d, tt }; // A2

            indices[i_index + 0] = v_index + 0; // cA1
            indices[i_index + 1] = v_index + 1; // cA2
            indices[i_index + 2] = v_index + 2; // A1
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + 1; // cA1
            indices[i_index + 1] = v_index + 2; // A1
            indices[i_index + 2] = v_index + 3; // A2
            i_index += 3; i_count += 3;

            v_index += 4;
        }

        vertices[v_index + 0] = vertex_t{ A1.x(), A1.y(), 0, r, g, b, a, 0, +d, t }; // A1
        vertices[v_index + 1] = vertex_t{ A2.x(), A2.y(), 0, r, g, b, a, 0, -d, t }; // A2
        vertices[v_index + 2] = vertex_t{ I.x(), I.y(), 0, r, g, b, a, 0, +d, t }; // I
        vertices[v_index + 3] = vertex_t{ J.x(), J.y(), 0, r, g, b, a, 0, +d, t }; // J
        vertices[v_index + 4] = vertex_t{ K.x(), K.y(), 0, r, g, b, a, 0, +d, t }; // K
        vertices[v_index + 5] = vertex_t{ L.x(), L.y(), 0, r, g, b, a, 0, -d, t }; // L
        v_count += 6;

        indices[i_index + 0] = v_index + 0; // A1
        indices[i_index + 1] = v_index + 1; // A2
        indices[i_index + 2] = v_index + 5; // L
        i_index += 3; i_count += 3;

        indices[i_index + 0] = v_index + 0; // A1
        indices[i_index + 1] = v_index + 3; // J
        indices[i_index + 2] = v_index + 5; // L
        i_index += 3; i_count += 3;

        /* Bevel joints */
        if (join == bevel_join)
        {
            indices[i_index + 0] = v_index + 3; // J
            indices[i_index + 1] = v_index + 4; // K
            indices[i_index + 2] = v_index + 5; // L
            i_index += 3; i_count += 3;
        }

        /* Miter joints */
        else if (join == miter_join)
        {
            indices[i_index + 0] = v_index + 2; // I
            indices[i_index + 1] = v_index + 3; // J
            indices[i_index + 2] = v_index + 5; // L
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + 2; // I
            indices[i_index + 1] = v_index + 4; // K
            indices[i_index + 2] = v_index + 5; // L
            i_index += 3; i_count += 3;
        }

        /* Round joints */
        else {
            float jk = Point2d(J.x() - K.x(), J.y() - K.y()).dLen();
            float jb = Point2d(J.x() - B.x(), J.y() - B.y()).dLen();
            float ib = Point2d(I.x() - B.x(), I.y() - B.y()).dLen();
            float jl = Point2d( J.x() - L.x(), J.y() - L.y()).dLen();
            float d1 = ib / jb;
            float d2 = 0.5*jk / jb;
            float d3 = sqrt(1 - d2*d2);
            float c = w / jl;
            d1 *= d; d2 *= d; d3 *= d;

            vertices[v_index + 6] = vertex_t{ I.x(), I.y(), 0, r, g, b, a, -d1, 0, t };    // I'
            vertices[v_index + 7] = vertex_t{ J.x(), J.y(), 0, r, g, b, a, -d3, +d2, t };    // J'
            vertices[v_index + 8] = vertex_t{ K.x(), K.y(), 0, r, g, b, a, -d3, -d2, t };    // K'
            vertices[v_index + 9] = vertex_t{ L.x(), L.y(), 0, r, g, b, a, 2 * c*d - d3, 0, t }; // L'
            v_count += 4;

            indices[i_index + 0] = v_index + 6; // I'
            indices[i_index + 1] = v_index + 7; // J'
            indices[i_index + 2] = v_index + 8; // K'
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + 7; // J'
            indices[i_index + 1] = v_index + 8; // K'
            indices[i_index + 2] = v_index + 9; // L'
            i_index += 3; i_count += 3;
        }


        if (i == (n_points - 3))
        {
            vertices[v_index + v_count + 0] = vertex_t{ C1.x(), C1.y(), 0, r, g, b, a, 0, +d, t };    // C1
            vertices[v_index + v_count + 1] = vertex_t{ C2.x(), C2.y(), 0, r, g, b, a, 0, -d, t };    // C2
            v_count += 2;

            indices[i_index + 0] = v_index + v_count - 2 + 0; // C1
            indices[i_index + 1] = v_index + v_count - 2 + 1; // C2
            indices[i_index + 2] = v_index + 5;           // L
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + v_count - 2 + 0; // C1
            indices[i_index + 1] = v_index + 5;           // L
            indices[i_index + 2] = v_index + 4;           // K
            i_index += 3; i_count += 3;
        }

        v_index += v_count;
        if (cross == 2)
        {
            A1 = K;
            A2 = L;
        }
        else
        {
            A1 = L;
            A2 = K;
        }
    }

    PVbo vbo(new Vbo());
    vbo->create(n_vertices, sizeof(vertex_t), vertices, n_indices, indices, Vbo::SOffsets(Vbo::E_VTYPE_VCT_F));
    return vbo;
}

//============================================================================
//============================================================================
PVbo UtlGlLine::createLined(const ListPt2d &points, float thickness, int join, int cap, float alpha)
{
    typedef struct { double x, y, z; float r, g, b, a, s, t, u; } vertex_t;

    int n_points = (int)points.size();
    float d, w;

    float r = 1;
    float g = 1;
    float b = 1;
    float a = alpha;
    float t = thickness;

    if (thickness < 1.0)
    {
        w = 2.0;
        a *= thickness;
        d = (w + 2) / w;
    }
    else
    {
        d = (thickness + 2.0) / thickness;
        w = thickness + 2.0;
    }


    size_t n_vertices = 0;
    size_t n_indices = 0;
    if ((join == bevel_join))
    {
        n_vertices = 2 + (n_points - 2) * 6 + 4;
        n_indices = 2 * 3 + (n_points - 2) * 3 * 3 + 6;
    }
    else if (join == miter_join)
    {
        n_vertices = 2 + (n_points - 2) * 6 + 4;
        n_indices = 2 * 3 + (n_points - 2) * 4 * 3 + 6;
    }
    else
    {
        n_vertices = 2 + (n_points - 2) * 10 + 4;
        n_indices = 2 * 3 + (n_points - 2) * 6 * 3 + 6;
    }
    vertex_t * vertices = (vertex_t *)calloc(n_vertices, sizeof(vertex_t));
    GLuint * indices = (GLuint *)calloc(n_indices, sizeof(GLuint));

    Point2d A, B, C;
    Point2d A1, A2, C1, C2;
    Point2d T_ba, O_ab, T_bc, O_cb;
    Point2d I, J, K, L, T;
    GLuint i;
    GLuint v_index = 0;
    GLuint v_count = 0;
    GLuint i_index = 0;
    GLuint i_count = 0;
    for (i = 0; i< (n_points - 2); ++i)
    {
        v_count = 0;
        i_count = 0;

        A = points[i + 0];
        B = points[i + 1];
        C = points[i + 2];
        T_ba = Point2d(B.x() - A.x(), B.y() - A.y());
        T_bc = Point2d(B.x() - C.x(), B.y() - C.y());
        O_ab = Point2d::ortho(A, B);
        O_cb = Point2d::ortho(C, B);
        C1 = Point2d(C.x() - w / 2 * O_cb.x(), C.y() - w / 2 * O_cb.y());
        C2 = Point2d(C.x() + w / 2 * O_cb.x(), C.y() + w / 2 * O_cb.y());
        if (i == 0)
        {
            A1 = Point2d(A.x() + w / 2 * O_ab.x(), A.y() + w / 2 * O_ab.y());
            A2 = Point2d(A.x() - w / 2 * O_ab.x(), A.y() - w / 2 * O_ab.y());
        }

        Point2d::intersection(A1, Point2d(A1.x() + T_ba.x(), A1.y() + T_ba.y()), C1, Point2d(C1.x() + T_bc.x(), C1.y() + T_bc.y()), &I);
        int cross = Point2d::intersection(A2, Point2d(A2.x() + T_ba.x(), A2.y() + T_ba.y()), C2, Point2d(C2.x() + T_bc.x(), C2.y() + T_bc.y()), &L);

        if (cross == 2)
        {
            J = Point2d(B.x() + w / 2 * O_ab.x(), B.y() + w / 2 * O_ab.y());
            K = Point2d(B.x() - w / 2 * O_cb.x(), B.y() - w / 2 * O_cb.y());
        }
        else
        {
            T = I; I = L; L = T;
            T = A1; A1 = A2; A2 = T;
            T = C1; C1 = C2; C2 = T;
            J = Point2d(B.x() - w / 2 * O_ab.x(), B.y() - w / 2 * O_ab.y());
            K = Point2d(B.x() + w / 2 * O_cb.x(), B.y() + w / 2 * O_cb.y());
        }

        // Cap start
        if (i == 0)
        {
            Point2d T = Point2d::tangent(B, A);
            Point2d cA1 = Point2d(A1.x() + w / 2 * T.x(), A1.y() + w / 2 * T.y());
            Point2d cA2 = Point2d(A2.x() + w / 2 * T.x(), A2.y() + w / 2 * T.y());

            double tt = t;
            if ((cap == square_cap) || (cap == butt_cap))
            {
                tt = -t;
            }

            vertices[v_index + 0] = vertex_t{ cA1.x(), cA1.y(), 0, r, g, b, a, -d, +d, tt }; // cA1
            vertices[v_index + 1] = vertex_t{ cA2.x(), cA2.y(), 0, r, g, b, a, -d, -d, tt }; // cA2
            vertices[v_index + 2] = vertex_t{ A1.x(), A1.y(), 0, r, g, b, a, 0, +d, tt }; // A1
            vertices[v_index + 3] = vertex_t{ A2.x(), A2.y(), 0, r, g, b, a, 0, -d, tt }; // A2

            indices[i_index + 0] = v_index + 0; // cA1
            indices[i_index + 1] = v_index + 1; // cA2
            indices[i_index + 2] = v_index + 2; // A1
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + 1; // cA1
            indices[i_index + 1] = v_index + 2; // A1
            indices[i_index + 2] = v_index + 3; // A2
            i_index += 3; i_count += 3;

            v_index += 4;
        }

        vertices[v_index + 0] = vertex_t{ A1.x(), A1.y(), 0, r, g, b, a, 0, +d, t }; // A1
        vertices[v_index + 1] = vertex_t{ A2.x(), A2.y(), 0, r, g, b, a, 0, -d, t }; // A2
        vertices[v_index + 2] = vertex_t{ I.x(), I.y(), 0, r, g, b, a, 0, +d, t }; // I
        vertices[v_index + 3] = vertex_t{ J.x(), J.y(), 0, r, g, b, a, 0, +d, t }; // J
        vertices[v_index + 4] = vertex_t{ K.x(), K.y(), 0, r, g, b, a, 0, +d, t }; // K
        vertices[v_index + 5] = vertex_t{ L.x(), L.y(), 0, r, g, b, a, 0, -d, t }; // L
        v_count += 6;

        indices[i_index + 0] = v_index + 0; // A1
        indices[i_index + 1] = v_index + 1; // A2
        indices[i_index + 2] = v_index + 5; // L
        i_index += 3; i_count += 3;

        indices[i_index + 0] = v_index + 0; // A1
        indices[i_index + 1] = v_index + 3; // J
        indices[i_index + 2] = v_index + 5; // L
        i_index += 3; i_count += 3;

        /* Bevel joints */
        if (join == bevel_join)
        {
            indices[i_index + 0] = v_index + 3; // J
            indices[i_index + 1] = v_index + 4; // K
            indices[i_index + 2] = v_index + 5; // L
            i_index += 3; i_count += 3;
        }

        /* Miter joints */
        else if (join == miter_join)
        {
            indices[i_index + 0] = v_index + 2; // I
            indices[i_index + 1] = v_index + 3; // J
            indices[i_index + 2] = v_index + 5; // L
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + 2; // I
            indices[i_index + 1] = v_index + 4; // K
            indices[i_index + 2] = v_index + 5; // L
            i_index += 3; i_count += 3;
        }

        /* Round joints */
        else {
            double jk = Point2d(J.x() - K.x(), J.y() - K.y()).dLen();
            double jb = Point2d(J.x() - B.x(), J.y() - B.y()).dLen();
            double ib = Point2d(I.x() - B.x(), I.y() - B.y()).dLen();
            double jl = Point2d(J.x() - L.x(), J.y() - L.y()).dLen();
            double d1 = ib / jb;
            double d2 = 0.5*jk / jb;
            double d3 = sqrt(1 - d2*d2);
            double c = w / jl;
            d1 *= d; d2 *= d; d3 *= d;

            vertices[v_index + 6] = vertex_t{ I.x(), I.y(), 0, r, g, b, a, -d1, 0, t };    // I'
            vertices[v_index + 7] = vertex_t{ J.x(), J.y(), 0, r, g, b, a, -d3, +d2, t };    // J'
            vertices[v_index + 8] = vertex_t{ K.x(), K.y(), 0, r, g, b, a, -d3, -d2, t };    // K'
            vertices[v_index + 9] = vertex_t{ L.x(), L.y(), 0, r, g, b, a, 2 * c*d - d3, 0, t }; // L'
            v_count += 4;

            indices[i_index + 0] = v_index + 6; // I'
            indices[i_index + 1] = v_index + 7; // J'
            indices[i_index + 2] = v_index + 8; // K'
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + 7; // J'
            indices[i_index + 1] = v_index + 8; // K'
            indices[i_index + 2] = v_index + 9; // L'
            i_index += 3; i_count += 3;
        }


        if (i == (n_points - 3))
        {
            vertices[v_index + v_count + 0] = vertex_t{ C1.x(), C1.y(), 0, r, g, b, a, 0, +d, t };    // C1
            vertices[v_index + v_count + 1] = vertex_t{ C2.x(), C2.y(), 0, r, g, b, a, 0, -d, t };    // C2
            v_count += 2;

            indices[i_index + 0] = v_index + v_count - 2 + 0; // C1
            indices[i_index + 1] = v_index + v_count - 2 + 1; // C2
            indices[i_index + 2] = v_index + 5;           // L
            i_index += 3; i_count += 3;

            indices[i_index + 0] = v_index + v_count - 2 + 0; // C1
            indices[i_index + 1] = v_index + 5;           // L
            indices[i_index + 2] = v_index + 4;           // K
            i_index += 3; i_count += 3;
        }

        v_index += v_count;
        if (cross == 2)
        {
            A1 = K;
            A2 = L;
        }
        else
        {
            A1 = L;
            A2 = K;
        }
    }

    Vbo::SDataTypes dt(GL_FLOAT);
    dt.vert = GL_DOUBLE;
    PVbo vbo(new Vbo());
    vbo->create(n_vertices, sizeof(vertex_t), vertices, n_indices, indices, Vbo::SOffsets(Vbo::E_VTYPE_VCT_D), dt);
    return vbo;
}

