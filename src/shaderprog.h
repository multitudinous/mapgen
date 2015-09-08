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
    void loadVertStr(const GLcharARB *pacShdrSrc);
    void loadFragStr(const GLcharARB *pacShdrSrc);

    void setTextureLoc(const GLcharARB *acSamplerName, GLint iTexLoc = 0);
    GLint getUniformLoc(const GLcharARB *name);

    virtual bool valid();
    virtual void destroy();

    GLhandleARB progHndl();

protected:
    void load(LPCTSTR pacFile, Shader *shdr);
    void loadStr(const GLcharARB *pacShdrSrc, Shader *shdr);
    void attach(Shader *shdr);
    void remove(Shader *shdr);

protected:
    GLhandleARB _hProg;
    PShader _shdrVert;
    PShader _shdrFrag;
};

typedef shared_ptr<ShaderProg> PShaderProg;

#endif