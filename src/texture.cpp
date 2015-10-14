#include "texture.h"

//============================================================================
//============================================================================
Texture::Texture()
{
	m_glid = 0;
	m_filter = I_FILTER_TRILINEAR;
}

//============================================================================
//============================================================================
Texture::~Texture()
{
	destroy();
}

//============================================================================
//============================================================================
bool Texture::create(GLuint width, GLuint height, GLint wrap, int filter, bool genMipMap)
{
	shared_ptr<MemBuf> pImg(new MemBuf(4, width, height));
	return create(pImg, wrap, filter, genMipMap);
}

//============================================================================
//============================================================================
bool Texture::create(shared_ptr<MemBuf> pImg, GLint wrap, int filter, bool genMipMap)
{
	const char *func = "Texture::CreateTexture() -";

	destroy();

	m_filter = filter;
	if (pImg.get() == NULL)
	{
		LogError("%s Image is null", func);
		return false;
	}

	if (pImg->getSize() != 3 && pImg->getSize() != 4)
	{
		LogError("%s Image pixel size is unsupported: %d", func, pImg->getSize());
		return false;
	}
	if (pImg->getLenX() <= 0 || pImg->getLenY() <= 0)
	{
		LogError("%s Image width or height is zero (%d, %d)", func, pImg->getLenX(), pImg->getLenY());
		return false;
	}

	m_pImg = pImg;
	glGenTextures(1, &m_glid);
	glBindTexture(GL_TEXTURE_2D, m_glid);

    // get the format
    GLenum eFormat = GL_RGB;
    if (m_pImg->getSize() == 4) eFormat = GL_RGBA;

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

    glTexImage2D(GL_TEXTURE_2D, 0, m_pImg->getSize(), m_pImg->getLenX(), m_pImg->getLenY(), 0, eFormat, GL_UNSIGNED_BYTE, m_pImg->getBuf());
    if (m_filter != I_FILTER_NONE && genMipMap)
    {
        glGenerateMipmapEXT(GL_TEXTURE_2D);
    }

	return true;
}

//============================================================================
//============================================================================
bool Texture::createMultisamp(GLuint width, GLuint height, GLsizei samples)
{
    destroy();

    glGenTextures(1, &m_glid);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_glid);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    return true;
}

//============================================================================
//============================================================================
void Texture::destroy()
{
	if (m_glid)
	{
		glDeleteTextures(1, &m_glid);
		m_glid = 0;
	}

	m_pImg.reset();
}

//============================================================================
//============================================================================
GLuint Texture::getId()
{
	return m_glid;
}

//============================================================================
//============================================================================
GLuint Texture::getWidth()
{
	if (!m_pImg) return 0;

	return m_pImg->getLenX();
}

//============================================================================
//============================================================================
GLuint Texture::getHeight()
{
	if (!m_pImg) return 0;

	return m_pImg->getLenY();
}
