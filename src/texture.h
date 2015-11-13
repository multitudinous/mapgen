#ifndef TEXTURE_H
#define TEXTURE_H

#include "membuf.h"
#include "sysdefs.h"

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

	bool create(GLuint width, GLuint height, GLint wrap=GL_REPEAT, int filter=I_FILTER_TRILINEAR, bool genMipMap = true);
    bool create(std::shared_ptr<MemBuf> pImg, GLint wrap = GL_REPEAT, int filter = I_FILTER_TRILINEAR, bool genMipMap = true);
    bool createMultisamp(GLuint width, GLuint height, GLsizei samples);
	bool reCreate();
	void destroy();

	GLuint getId();
	GLuint getWidth();
	GLuint getHeight();

    MemBuf *img() { return m_pImg.get();  }

protected:
	GLuint m_glid;
	std::shared_ptr<MemBuf> m_pImg; // TODO: do we really want to keep this in memory (maybe for some image operations, see geoimg, geoimgraster
	int m_filter;
};

typedef std::shared_ptr<Texture> PTexture;

#endif
