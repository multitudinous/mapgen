#include "membuf.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

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
