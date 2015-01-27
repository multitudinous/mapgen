#ifndef UTLGL_H
#define UTLGL_H

#include "sysdefs.h"

class UtlGL
{
public:
	static string GetTextureInfo(GLuint id);
	static string GetRenderbufferInfo(GLuint id);
	static string ConvertGLFormatToString(GLenum format);
};

#endif
