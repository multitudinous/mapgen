#include "membuf.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
MemBuf::MemBuf()
{
	m_pbuf = NULL;
	m_size = 0;
	m_lenX = 0;
	m_lenY = 0;
}

MemBuf::MemBuf(int size, int lengthX, int lengthY)
{
	m_pbuf = NULL;
	m_size = 0;
	m_lenX = 0;

	SetSize(size, lengthX, lengthY);
}

MemBuf::~MemBuf()
{
	Destroy(); // TODO: WHY WAS THIS NOT HERE BEFORE
}

void MemBuf::SetSize(int size, int lengthX, int lengthY)
{
	Destroy();

	if (size < 0 || lengthX < 0 || lengthY < 0) return;

	m_size = size;
	m_lenX = lengthX;
	m_lenY = lengthY;
	m_pbuf = malloc(m_size*m_lenX*m_lenY);
}

void MemBuf::Destroy()
{
	if (m_pbuf)
	{
		free(m_pbuf);
	}

	m_lenX = 0;
	m_lenY = 0;
}
