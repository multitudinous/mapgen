#include "vbo.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

//============================================================================
//============================================================================
Vbo::Vbo() :
    _vbo(0),
    _ibo(0),
    _vertCount(0),
    _vertSize(0),
    _idxCount(0),
    _passes(1),
    _vertsPerPass(0),
    _primType(GL_TRIANGLES)
{
}

//============================================================================
//============================================================================
Vbo::~Vbo()
{
    destroy();
}

//============================================================================
// create just a vertex buffer
//
// todo: handle errors
//============================================================================
bool Vbo::create(int vertCount, int vertSize, void *pVerts, SOffsets ofs, SDataTypes dts, int primType, int passes, int vertsPerPass)
{
    GLenum err;

    err = glGetError();
    destroy();
    glGenBuffersARB(1, &_vbo);
    err = glGetError();

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, _vbo);
    err = glGetError();
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertCount*vertSize, pVerts, GL_STATIC_DRAW_ARB);
    err = glGetError();
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    err = glGetError();

    _vertCount = vertCount;
    _vertSize = vertSize;
    _ofs = ofs;
    _dts = dts;
    _primType = primType;
    _passes = passes;
    _vertsPerPass = vertsPerPass;
    if (_passes == 1 && _vertsPerPass == 0) _vertsPerPass = _vertCount;

    return true;
}

//============================================================================
// create with an index buffer
//
// todo: handle errors
//============================================================================
bool Vbo::create(int vertCount, int vertSize, void *pVerts, int idxCount, GLuint *pIdx, SOffsets ofs, SDataTypes dts, int primType, int passes, int vertsPerPass)
{
    // create the vbo
    if (!create(vertCount, vertSize, pVerts, ofs, dts, primType, passes, vertsPerPass))
    {
        return false;
    }


    GLenum err;

    // create the ibo
    err = glGetError();
    glGenBuffersARB(1, &_ibo);
    err = glGetError();

    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    err = glGetError();
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, idxCount*sizeof(GLuint), pIdx, GL_STATIC_DRAW_ARB);
    err = glGetError();
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
    err = glGetError();

    _idxCount = idxCount;
    return true;
}


//============================================================================
//============================================================================
void Vbo::destroy()
{
    if (_vbo)
    {
        glDeleteBuffersARB(1, &_vbo);
        _vbo = 0;
    }

    if (_ibo)
    {
        glDeleteBuffersARB(1, &_ibo);
        _ibo = 0;
    }

    _vertCount = 0;
    _vertSize = 0;
    _idxCount = 0;
}

//============================================================================
//============================================================================
void Vbo::pointersEnable()
{
    if (_ofs.vert > -1)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, _dts.vert, _vertSize, BUFFER_OFFSET(_ofs.vert));
    }
    if (_ofs.norm > -1)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(_dts.norm, _vertSize, BUFFER_OFFSET(_ofs.norm));
    }
    if (_ofs.colr > -1)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, _dts.colr, _vertSize, BUFFER_OFFSET(_ofs.colr));
    }
    if (_ofs.tex0 > -1)
    {
        glClientActiveTextureARB(GL_TEXTURE0_ARB);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        if (_ofs.vtype == E_VTYPE_VCT_F)
        {
            glTexCoordPointer(3, _dts.tex, _vertSize, BUFFER_OFFSET(_ofs.tex0));
        }
        else
        {
            glTexCoordPointer(2, _dts.tex, _vertSize, BUFFER_OFFSET(_ofs.tex0));
        }
    }
    if (_ofs.tex1 > -1)
    {
        glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, _dts.tex, _vertSize, BUFFER_OFFSET(_ofs.tex1));
    }
}

//============================================================================
//============================================================================
void Vbo::pointersDisable()
{
    if (_ofs.vert > -1)
    {
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    if (_ofs.norm > -1)
    {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (_ofs.colr > -1)
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }
    if (_ofs.tex1 > -1)
    {
        glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if (_ofs.tex0 > -1)
    {
        glClientActiveTextureARB(GL_TEXTURE0_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

//============================================================================
// todo: handle errors
//============================================================================
void Vbo::draw()
{
    GLenum err;
    bool useIbo = false;

    if (!_vbo) return;

    err = glGetError();
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, _vbo);
    err = glGetError();

    if (_ibo && _idxCount > 0)
    {
        useIbo = true;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        err = glGetError();
    }
    

    pointersEnable();
    err = glGetError();

    int offset = 0;
    for (int i = 0; i<_passes; i++)
    {
        if (useIbo)
        {
            glDrawElements(_primType, _idxCount, GL_UNSIGNED_INT, (void*)0);
        }
        else
        {
            glDrawArrays(_primType, offset, _vertsPerPass);
        }

        offset += _vertsPerPass;
    }


    pointersDisable();
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
}

//============================================================================
//============================================================================
int Vbo::getVertSize(int vtype)
{
    switch (vtype)
    {
    case E_VTYPE_V_F:
        return sizeof(SV_F);
        break;
    case E_VTYPE_VT_F:
        return sizeof(SVT_F);
        break;
    case E_VTYPE_VC_F:
        return sizeof(SVC_F);
        break;
    case E_VTYPE_VCT_F:
        return sizeof(SVCT_F);
        break;
    case E_VTYPE_VNC_F:
        return sizeof(SVNC_F);
        break;
    case E_VTYPE_VNCT_F:
        return sizeof(SVNTC_F);
        break;
    }

    return 0;
}