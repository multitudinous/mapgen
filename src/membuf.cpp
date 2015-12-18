#include "membuf.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <cstring>

//============================================================================
//============================================================================
MemBuf::MemBuf()
{
	m_pbuf = NULL;
	m_size = 0;
	m_lenX = 0;
	m_lenY = 0;
}

//============================================================================
//============================================================================
MemBuf::MemBuf(int size, int lengthX, int lengthY)
{
	m_pbuf = NULL;
	m_size = 0;
	m_lenX = 0;

	setSize(size, lengthX, lengthY);
}

//============================================================================
//============================================================================
MemBuf::~MemBuf()
{
	destroy(); // TODO: WHY WAS THIS NOT HERE BEFORE
}

//============================================================================
//============================================================================
void MemBuf::setSize(int size, int lengthX, int lengthY)
{
	destroy();

	if (size < 0 || lengthX < 0 || lengthY < 0) return;

	m_size = size;
	m_lenX = lengthX;
	m_lenY = lengthY;
	m_pbuf = malloc(m_size*m_lenX*m_lenY);
}

//============================================================================
//============================================================================
void MemBuf::destroy()
{
	if (m_pbuf)
	{
		free(m_pbuf);
	}

	m_lenX = 0;
	m_lenY = 0;
}

//============================================================================
//============================================================================
void MemBuf::copy(const MemBuf &buf, bool flipY, bool flipA)
{
    setSize(buf.getSize(), buf.getLenX(), buf.getLenY());

    const unsigned char *src;
    unsigned char *dst;
    int linelen = getLenX() * getSize();
    int bytesperpix = getSize();

    for (int y = 0; y < getLenY(); y++)
    {
        dst = (unsigned char *)getBuf() + y * linelen;

        int srcpos = y * linelen;
        if (flipY)
        {
            srcpos = (buf.getLenY() - y - 1) * linelen;
        }
        
        src = (const unsigned char *)buf.getBufRead() + srcpos;

        for (int x = 0; x < getLenX(); x++)
        {
            if (flipA && bytesperpix==4)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = src[3];
                /*
                dst[0] = src[3];
                dst[3] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                */
            }
            else
            {
                memcpy(dst, src, bytesperpix);
            }

            src += bytesperpix;
            dst += bytesperpix;
        }
    }



}
