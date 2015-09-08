#include "utlgl.h"

string UtlGL::GetTextureInfo(GLuint id)
{
    if (glIsTexture(id) == GL_FALSE) return "Not texture object";

    int width, height, format;
    string formatName;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
    glBindTexture(GL_TEXTURE_2D, 0);

    formatName = ConvertGLFormatToString(format);

	std::stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}

string UtlGL::GetRenderbufferInfo(GLuint id)
{
    if (glIsRenderbufferEXT(id) == GL_FALSE) return "Not Renderbuffer object";

    int width, height, format;
    std::string formatName;
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id);
    glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &width);    // get renderbuffer width
    glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &height);  // get renderbuffer height
    glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_INTERNAL_FORMAT_EXT, &format); // get renderbuffer internal format
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    formatName = ConvertGLFormatToString(format);

    std::stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////
// convert OpenGL internal format enum to string
///////////////////////////////////////////////////////////////////////////////
string UtlGL::ConvertGLFormatToString(GLenum format)
{
    std::string formatName;

    switch(format)
    {
    case GL_STENCIL_INDEX:
        formatName = "GL_STENCIL_INDEX";
        break;
    case GL_DEPTH_COMPONENT:
        formatName = "GL_DEPTH_COMPONENT";
        break;
    case GL_ALPHA:
        formatName = "GL_ALPHA";
        break;
    case GL_RGB:
        formatName = "GL_RGB";
        break;
    case GL_RGBA:
        formatName = "GL_RGBA";
        break;
    case GL_LUMINANCE:
        formatName = "GL_LUMINANCE";
        break;
    case GL_LUMINANCE_ALPHA:
        formatName = "GL_LUMINANCE_ALPHA";
        break;
    case GL_ALPHA4:
        formatName = "GL_ALPHA4";
        break;
    case GL_ALPHA8:
        formatName = "GL_ALPHA8";
        break;
    case GL_ALPHA12:
        formatName = "GL_ALPHA12";
        break;
    case GL_ALPHA16:
        formatName = "GL_ALPHA16";
        break;
    case GL_LUMINANCE4:
        formatName = "GL_LUMINANCE4";
        break;
    case GL_LUMINANCE8:
        formatName = "GL_LUMINANCE8";
        break;
    case GL_LUMINANCE12:
        formatName = "GL_LUMINANCE12";
        break;
    case GL_LUMINANCE16:
        formatName = "GL_LUMINANCE16";
        break;
    case GL_LUMINANCE4_ALPHA4:
        formatName = "GL_LUMINANCE4_ALPHA4";
        break;
    case GL_LUMINANCE6_ALPHA2:
        formatName = "GL_LUMINANCE6_ALPHA2";
        break;
    case GL_LUMINANCE8_ALPHA8:
        formatName = "GL_LUMINANCE8_ALPHA8";
        break;
    case GL_LUMINANCE12_ALPHA4:
        formatName = "GL_LUMINANCE12_ALPHA4";
        break;
    case GL_LUMINANCE12_ALPHA12:
        formatName = "GL_LUMINANCE12_ALPHA12";
        break;
    case GL_LUMINANCE16_ALPHA16:
        formatName = "GL_LUMINANCE16_ALPHA16";
        break;
    case GL_INTENSITY:
        formatName = "GL_INTENSITY";
        break;
    case GL_INTENSITY4:
        formatName = "GL_INTENSITY4";
        break;
    case GL_INTENSITY8:
        formatName = "GL_INTENSITY8";
        break;
    case GL_INTENSITY12:
        formatName = "GL_INTENSITY12";
        break;
    case GL_INTENSITY16:
        formatName = "GL_INTENSITY16";
        break;
    case GL_R3_G3_B2:
        formatName = "GL_R3_G3_B2";
        break;
    case GL_RGB4:
        formatName = "GL_RGB4";
        break;
    case GL_RGB5:
        formatName = "GL_RGB4";
        break;
    case GL_RGB8:
        formatName = "GL_RGB8";
        break;
    case GL_RGB10:
        formatName = "GL_RGB10";
        break;
    case GL_RGB12:
        formatName = "GL_RGB12";
        break;
    case GL_RGB16:
        formatName = "GL_RGB16";
        break;
    case GL_RGBA2:
        formatName = "GL_RGBA2";
        break;
    case GL_RGBA4:
        formatName = "GL_RGBA4";
        break;
    case GL_RGB5_A1:
        formatName = "GL_RGB5_A1";
        break;
    case GL_RGBA8:
        formatName = "GL_RGBA8";
        break;
    case GL_RGB10_A2:
        formatName = "GL_RGB10_A2";
        break;
    case GL_RGBA12:
        formatName = "GL_RGBA12";
        break;
    case GL_RGBA16:
        formatName = "GL_RGBA16";
        break;
    default:
        formatName = "Unknown Format";
    }

    return formatName;
}

//=======================================================================
//=======================================================================
string UtlGL::getLog(GLhandleARB handle)
{
    int blen = 0, slen = 0;
    std::vector<GLcharARB> log;
    glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &blen);

    if (blen <= 1) return string("");
    
    log.resize(blen + 1);
    log[blen] = 0;
    glGetInfoLogARB(handle, blen, &slen, &log[0]);
    return std::string(&log[0]);
}

//=======================================================================
//=======================================================================
void UtlGL::logErrorCheck(const char *msg)
{
    GLenum e = glGetError();
    if (e == GL_NO_ERROR) return;

    logError(e, msg);
}

//=======================================================================
//=======================================================================
void UtlGL::logError(GLenum e, const char *msg)
{
    switch (e)
    {
    case GL_NO_ERROR:
        break;
    case GL_INVALID_ENUM:
        LogError("%s - GL error: 0x%X - Invalid Enum", msg, e);
        break;
    case GL_INVALID_VALUE:
        LogError("%s - GL error: 0x%X - Invalid Value", msg, e);
        break;
    case GL_INVALID_OPERATION:
        LogError("%s - GL error: 0x%X - Invalid Operation", msg, e);
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        LogError("%s - GL error: 0x%X - Invalid Fbo Operation", msg, e);
        break;
    case GL_OUT_OF_MEMORY:
        LogError("%s - GL error: 0x%X - Out of Memory", msg, e);
        break;
    case GL_STACK_UNDERFLOW:
        LogError("%s - GL error: 0x%X - Stack Underflow", msg, e);
        break;
    case GL_STACK_OVERFLOW:
        LogError("%s - GL error: 0x%X - Stack Overflow", msg, e);
        break;
    default:
        LogError("%s - GL error: 0x%X - UnKnown Error", msg, e);
        break;

    }
}