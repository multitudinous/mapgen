#pragma once

#include "sysdefs.h"
#include <memory>

struct SV_F
{
    float x, y, z;
};

struct SVT_F
{
    float x, y, z;
    float u, v;
};

struct SVC_F
{
    float x, y, z;
    float r, g, b, a;
};

// for antialiased polylines
struct SVCT_F
{
    float x, y, z;
    float r, g, b, a;
    float s, t, u;       // x,y,z tx coords, or u, v, s
};

struct SVCT_D
{
    double x, y, z;
    float r, g, b, a;
    float s, t, u;       // x,y,z tx coords, or u, v, s
};


struct SVNC_F
{
    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
};

struct SVNTC_F
{
    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
    float u, v;
};

struct SVNTC_D
{
    double x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
    float u, v;
};

class Vbo
{
public:

    enum EVType
    {
        E_VTYPE_UNK = 0,
        E_VTYPE_V_F = 1,
        E_VTYPE_VT_F,
        E_VTYPE_VC_F,
        E_VTYPE_VCT_F,
        E_VTYPE_VCT_D,
        E_VTYPE_VNC_F,
        E_VTYPE_VNCT_F,
        E_VTYPE_VNCTT_F,

    };

    struct SDataTypes
    {
        GLenum vert;
        GLenum norm;
        GLenum colr;
        GLenum tex;

        SDataTypes(GLenum etype = GL_FLOAT)
        {
            vert = etype;
            norm = etype;
            colr = etype;
            tex = etype;
        }
    };

    struct SOffsets
    {
        int vert;
        int norm;
        int colr;
        int tex0;
        int tex1;
        int tex2;
        int tex3;
        EVType vtype;

        SOffsets()
        {
            vtype = E_VTYPE_UNK;
            clear();
        }

        SOffsets(EVType etype)
        {
            vtype = etype;
            switch (etype)
            {
            case E_VTYPE_V_F:
                setV_F();
                break;
            case E_VTYPE_VT_F:
                setVT_F();
                break;
            case E_VTYPE_VC_F:
                setVC_F();
                break;
            case E_VTYPE_VCT_F:
                setVCT_F();
                break;
            case E_VTYPE_VCT_D:
                setVCT_D();
                break;
            case E_VTYPE_VNC_F:
                setVNC_F();
                break;
            case E_VTYPE_VNCT_F:
                setVNCT_F();
                break;
            default:
                clear();
            }
        }

        void setV_F()
        {
            clear();
            vert = 0;
        }

        void setVT_F()
        {
            clear();
            vert = 0;
            tex0 = sizeof(GLfloat) * 3;
        }

        void setVC_F()
        {
            clear();
            vert = 0;
            colr = sizeof(GLfloat) * 3;
        }

        void setVCT_F()
        {
            clear();
            vert = 0;
            colr = sizeof(GLfloat) * 3;
            tex0 = colr + sizeof(GLfloat)*4;
        }

        void setVCT_D()
        {
            clear();
            vert = 0;
            colr = sizeof(GLdouble) * 3;
            tex0 = colr + sizeof(GLfloat) * 4;
        }

        void setVNC_F()
        {
            clear();
            vert = 0;
            norm = sizeof(GLfloat) * 3;
            colr = norm + sizeof(GLfloat) * 3;
        }

        void setVNCT_F()
        {
            clear();
            vert = 0;
            norm = sizeof(GLfloat) * 3;
            colr = norm + sizeof(GLfloat) * 3;
            tex0 = colr + sizeof(GLfloat) * 4;
        }

        void clear()
        {
            vert = -1;
            norm = -1;
            colr = -1;
            tex0 = -1;
            tex1 = -1;
            tex2 = -1;
            tex3 = -1;

        }
    };

public:
    Vbo();
    ~Vbo();

    bool create(int vertCount, int vertSize, void *pVerts, SOffsets ofs = SOffsets(E_VTYPE_VNCT_F), SDataTypes dts = SDataTypes(), int primType = GL_TRIANGLES, int passes = 1, int vertsPerPass = 0);
    bool create(int vertCount, int vertSize, void *pVerts, int idxCount, GLuint *pIdx, SOffsets ofs = SOffsets(E_VTYPE_VNCT_F), SDataTypes dts = SDataTypes(), int primType = GL_TRIANGLES, int passes = 1, int vertsPerPass = 0); // create with index buffer
    void destroy();

    void draw();

    static int getVertSize(int vtype);

protected:
    void pointersEnable();
    void pointersDisable();

protected:
    GLuint _vbo; // vbo id
    GLuint _ibo; // index buffer id

    int _vertCount;
    int _vertSize;
    int _idxCount;
    SDataTypes _dts;
    SOffsets _ofs;
    int _primType;
    int _passes;
    int _vertsPerPass;

    // TODO: add support for multiple index buffers within, so we can uses less buffer objects, which is more effecient.
    

};

typedef std::shared_ptr<Vbo> PVbo;
