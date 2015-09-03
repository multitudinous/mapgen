#ifndef SHADER_H
#define SHADER_H

#include "sysdefs.h"

class Shader
{
public:
    Shader(GLenum etype = GL_VERTEX_SHADER_ARB); // GL_FRAGMENT_SHADER_ARB
    virtual ~Shader();

    virtual void load(const char *file);
    virtual void load(const GLcharARB *pacShdrSrc, GLint iLen);
    virtual void handle(GLhandleARB hShdr) { _hShdr = hShdr; }
    virtual GLhandleARB handle() { return _hShdr; }

    virtual bool valid();
    virtual void destroy();

protected:
    GLhandleARB _hShdr;
    GLenum _eType;
};

typedef std::shared_ptr<Shader> PShader;

#endif
