#ifndef TEXTURE_H
#define TEXTURE_H

#include "membuf.h"

class Texture
{
public:
	// texture filter types
    enum
    {
		I_FILTER_NONE      = 1, 
		I_FILTER_LINEAR    = 2,
		I_FILTER_BILINEAR  = 3,
		I_FILTER_TRILINEAR = 4,
		I_FILTER_DEPTHMAP  = 5,
    };

public:
	Texture();
	virtual ~Texture();

	bool Create(GLuint width, GLuint height, GLint wrap=GL_REPEAT, int filter=I_FILTER_TRILINEAR);
	bool Create(shared_ptr<MemBuf> pImg, GLint wrap=GL_REPEAT, int filter=I_FILTER_TRILINEAR);
	bool ReCreate();
	void Destroy();

	GLuint GetId();
	GLuint GetWidth();
	GLuint GetHeight();

protected:
	GLuint m_glid;
	shared_ptr<MemBuf> m_pImg; // TODO: do we really want to keep this in memory
	int m_filter;
};

typedef shared_ptr<Texture> PTexture;

#endif
