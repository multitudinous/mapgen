#ifndef SHADERPROG_H
#define SHADERPROG_H

#include "sysdefs.h"
#include "shader.h"

class ShaderProg
{
public:
    ShaderProg();
    virtual ~ShaderProg();

    bool enable();
    void disable();

    void loadVert(const char *pacFile);
    void loadFrag(const char *pacFile);
    void loadVert(const GLcharARB *pacShdrSrc, GLint iLen); // must be null terminated if iLen == 0
    void loadFrag(const GLcharARB *pacShdrSrc, GLint iLen); // must be null terminated if iLen == 0

    void setTextureLoc(const GLcharARB *acSamplerName, GLint iTexLoc = 0);

    virtual bool valid();
    virtual void destroy();

    GLhandleARB progHndl();

protected:
    void load(LPCTSTR pacFile, Shader *shdr);
    void load(const GLcharARB *pacShdrSrc, GLint iLen, Shader *shdr);
    void attach(Shader *shdr);
    void remove(Shader *shdr);

protected:
    GLhandleARB _hProg;
    PShader _shdrVert;
    PShader _shdrFrag;
};

typedef shared_ptr<ShaderProg> PShaderProg;

#endif