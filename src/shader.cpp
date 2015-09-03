#include "shader.h"
#include <iostream>
#include <fstream>
#include "utlgl.h"

//============================================================================
//============================================================================
Shader::Shader(GLenum etype) :
    _hShdr(NULL),
    _eType(etype)
{

}

//============================================================================
//============================================================================
Shader::~Shader()
{
    destroy();
}


//=======================================================================
// throws runtime_error on failure
//=======================================================================
void Shader::load(const char *strfile)
{
    ifstream file(strfile, ios::in | ios::binary | ios::ate);
    if (!file.is_open())
    {
        std::string err = "Failed to open file: ";
        err += std::string(strfile);
        throw std::runtime_error(err);
    }

    size_t size = file.tellg();
    if (size <= 0)
    {
        file.close();

        std::string err = "File is empty: ";
        err += std::string(strfile);
        throw std::runtime_error(err);
    }

    std::vector<GLcharARB> src(size + 1);
    src[size] = 0; // terminate the string

    file.seekg(0, ios::beg);
    file.read(&src[0], size);
    file.close();

    load(&src[0], (GLint)src.size());
}

//=======================================================================
// throws runtime_error on failure
//=======================================================================
void Shader::load(const GLcharARB *pacShdrSrc, GLint iLen)
{
    if (_eType == 0)
    {
        throw std::runtime_error("Shader type is invalid!");
    }

    if (iLen <= 0)
    {
        throw std::runtime_error("Shader string is empty!");
    }

    if (!_hShdr)
    {
        _hShdr = glCreateShaderObjectARB(_eType);
    }
    if (_hShdr == 0)
    {
        throw std::runtime_error("Shader object is invalid!");
    }

   
    int compiled = 0;
    glShaderSourceARB(_hShdr, 1, &pacShdrSrc, &iLen);
    glCompileShaderARB(_hShdr);
    glGetObjectParameterivARB(_hShdr, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
    if (!compiled)
    {
        // compilation error! Check compiler log!
        std::string log = UtlGL::getLog(_hShdr);
        std::string err = "Shader failed to compile:\n  ";
        if (log.size() > 0)
        {
            err += log;
        }
        else
        {
            err += "An unknown error occured.";
        }

        /*
        int blen = 0, slen = 0;
        std::vector<GLcharARB> log;
        glGetObjectParameterivARB(_hShdr, GL_OBJECT_INFO_LOG_LENGTH_ARB, &blen);

        if (blen > 1)
        {
            log.resize(blen + 1);
            log[blen] = 0;
            glGetInfoLogARB(_hShdr, blen, &slen, &log[0]);
        }
        
        std::string err = "Shader failed to compile:\n  ";
        if (log.size() > 0)
        {
            err += std::string(&log[0]);
        }
        else
        {
            err += "An unknown error occured.";
        }
        */

        destroy();
        throw std::runtime_error(err);
    }
}

//=======================================================================
//=======================================================================
bool Shader::valid()
{
    if (_hShdr)
        return true;

    return false;
}

//=======================================================================
//=======================================================================
void Shader::destroy()
{
    if (!_hShdr) return;
    
    glDeleteObjectARB(_hShdr);
    _hShdr = NULL;
}