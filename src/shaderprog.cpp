#include "shaderprog.h"
#include "utlgl.h"

//============================================================================
//============================================================================
ShaderProg::ShaderProg() : 
_hProg(NULL),
_shdrVert(new Shader(GL_VERTEX_SHADER_ARB)),
_shdrFrag(new Shader(GL_FRAGMENT_SHADER_ARB))
{

}

//============================================================================
//============================================================================
ShaderProg::~ShaderProg()
{
    destroy();
}

//============================================================================
//============================================================================
bool ShaderProg::enable()
{
    if (!_hProg) return false;
    glUseProgramObjectARB(_hProg);
    return true;
}
//============================================================================
//============================================================================
void ShaderProg::disable()
{
    glUseProgramObjectARB(NULL);
}

//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::loadVert(const char *pacFile)
{
    if (!_shdrVert) _shdrVert.reset(new Shader(GL_VERTEX_SHADER_ARB));
    load(pacFile, _shdrVert.get());
}

//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::loadFrag(const char *pacFile)
{
    if (!_shdrFrag) _shdrFrag.reset(new Shader(GL_FRAGMENT_SHADER_ARB));
    load(pacFile, _shdrFrag.get());
}

//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::loadVert(const GLcharARB *pacShdrSrc, GLint iLen)
{
    if (!_shdrVert) _shdrVert.reset(new Shader(GL_VERTEX_SHADER_ARB));
    load(pacShdrSrc, iLen, _shdrVert.get());
}

//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::loadFrag(const GLcharARB *pacShdrSrc, GLint iLen)
{
    if (!_shdrFrag) _shdrFrag.reset(new Shader(GL_FRAGMENT_SHADER_ARB));
    load(pacShdrSrc, iLen, _shdrFrag.get());
}

//============================================================================
//============================================================================
void ShaderProg::setTextureLoc(const GLcharARB *acSamplerName, GLint iTexLoc)
{
    GLint loc = glGetUniformLocationARB(_hProg, acSamplerName);
    glUniform1iARB(loc, iTexLoc);
}

//============================================================================
//============================================================================
bool ShaderProg::valid()
{
    if (_hProg) return true;
    return false;
}

//============================================================================
//============================================================================
void ShaderProg::destroy()
{
    _shdrVert.reset();
    _shdrFrag.reset();
    if (_hProg)
    {
        glDeleteObjectARB(_hProg);
        _hProg = 0;
    }
}

//============================================================================
//============================================================================
GLhandleARB ShaderProg::progHndl()
{ 
    return _hProg; 
}

//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::load(LPCTSTR pacFile, Shader *shdr)
{
    remove(shdr);
    shdr->load(pacFile);
    attach(shdr);
}
//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::load(const GLcharARB *pacShdrSrc, GLint iLen, Shader *shdr)
{
    remove(shdr);
    shdr->load(pacShdrSrc, iLen);
    attach(shdr);
}
//============================================================================
// throws runtime exception on error
//============================================================================
void ShaderProg::attach(Shader *shdr)
{
    GLint linked = 0;

    if (!_hProg) _hProg = glCreateProgramObjectARB();

    glAttachObjectARB(_hProg, shdr->handle());
    glLinkProgramARB(_hProg);
    glGetObjectParameterivARB(_hProg, GL_OBJECT_LINK_STATUS_ARB, &linked);
    if (!linked)
    {
        // link error! Check compiler log! 
        std::string log = UtlGL::getLog(_hProg);

        std::string err = "Shader failed to link:\n  ";
        if (log.size() > 0) err += log;
        else err += "An unknown link error occured.";

        throw std::runtime_error(err);
    }
}
//============================================================================
//============================================================================
void ShaderProg::remove(Shader *shdr)
{
    if (!shdr || !shdr->valid()) return;

    if (_hProg)
    {
        glDetachObjectARB(_hProg, shdr->handle());
        shdr->destroy();
    }
}