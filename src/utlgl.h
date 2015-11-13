#ifndef UTLGL_H
#define UTLGL_H

#include "sysdefs.h"
#include "vector3.h"

class UtlGL
{
public:
	static string GetTextureInfo(GLuint id);
	static string GetRenderbufferInfo(GLuint id);
	static string ConvertGLFormatToString(GLenum format);

    static void getPickRay(int mousex, int mousey, vec3d *vstart, vec3d *vend, bool iswindows=true, int *mouseyAdjusted=NULL);

    static void overaly2dInit();
    static void overaly2dRestore();

    static string getLog(GLhandleARB handle);

    static void logErrorCheck(const char *msg);
    static void logError(GLenum e, const char *msg = "Error");
};

#endif
