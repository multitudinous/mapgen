#include "texture.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

Texture::Texture()
{
	m_glid = 0;
	m_filter = I_FILTER_TRILINEAR;
}

Texture::~Texture()
{
	Destroy();
}

bool Texture::Create(GLuint width, GLuint height, GLint wrap, int filter)
{
	shared_ptr<MemBuf> pImg(new MemBuf(4, width, height));
	return Create(pImg, wrap, filter);
}

bool Texture::Create(shared_ptr<MemBuf> pImg, GLint wrap, int filter)
{
	const char *func = "Texture::CreateTexture() -";

	Destroy();

	m_filter = filter;
	if (pImg.get() == NULL)
	{
		LogError("%s Image is null", func);
		return false;
	}

	if (pImg->GetSize() != 3 && pImg->GetSize() != 4)
	{
		LogError("%s Image pixel size is unsupported: %d", func, pImg->GetSize());
		return false;
	}
	if (pImg->GetLenX() <= 0 || pImg->GetLenY() <= 0)
	{
		LogError("%s Image width or height is zero (%d, %d)", func, pImg->GetLenX(), pImg->GetLenY());
		return false;
	}

	m_pImg = pImg;
	glGenTextures(1, &m_glid);
	glBindTexture(GL_TEXTURE_2D, m_glid);

    // get the format
    GLenum eFormat = GL_RGB;
    if (m_pImg->GetSize() == 4) eFormat = GL_RGBA;

    // create the mipmaps
	/*
    if (m_filter != I_FILTER_NONE)
    {
	// this will resize your texture if it is not a power of 2
		if (gluBuild2DMipmaps(GL_TEXTURE_2D, m_pImg->GetSize(), m_pImg->GetLenX(), m_pImg->GetLenY(), eFormat, GL_UNSIGNED_BYTE, m_pImg->GetBuf()))
		{
			LogError("%s gluBuild2DMipmaps failed", func);
		}
    }
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, m_pImg->GetSize(), m_pImg->GetLenX(), m_pImg->GetLenY(), 0, eFormat, GL_UNSIGNED_BYTE, m_pImg->GetBuf());
	}
	*/

	glTexImage2D(GL_TEXTURE_2D, 0, m_pImg->GetSize(), m_pImg->GetLenX(), m_pImg->GetLenY(), 0, eFormat, GL_UNSIGNED_BYTE, m_pImg->GetBuf());
	if (m_filter != I_FILTER_NONE) glGenerateMipmapEXT(GL_TEXTURE_2D);



	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	switch (m_filter)
	{
		case I_FILTER_NONE:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		case I_FILTER_LINEAR:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case I_FILTER_BILINEAR:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case I_FILTER_TRILINEAR:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		default:
			// BILINEAR:
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		  break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	return true;
}

void Texture::Destroy()
{
	if (m_glid)
	{
		glDeleteTextures(1, &m_glid);
		m_glid = 0;
	}

	m_pImg.reset();
}

GLuint Texture::GetId()
{
	return m_glid;
}

GLuint Texture::GetWidth()
{
	if (!m_pImg) return 0;

	return m_pImg->GetLenX();
}

GLuint Texture::GetHeight()
{
	if (!m_pImg) return 0;

	return m_pImg->GetLenY();
}
