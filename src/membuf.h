#ifndef MEMBUF_H
#define MEMBUF_H

#include <memory>

class MemBuf
{
public:
	MemBuf();
	MemBuf(int size, int lengthX, int lengthY=1);
	virtual ~MemBuf();

	void SetSize(int size, int lengthX, int lengthY=1);
	void Destroy();

	int GetSize() const { return m_size; } // size of a single element in bytes
	int GetLenX() const { return m_lenX; }
	int GetLenY() const { return m_lenY; }
	int GetByteCount() const { return m_size*m_lenX*m_lenY; }
	void* GetBuf() { return m_pbuf; }
	const void* GetBufRead() const { return m_pbuf; }

protected:
	void* m_pbuf;
	int m_size;
	int m_lenX;
	int m_lenY;
};

typedef std::shared_ptr<MemBuf> PMemBuf;

#endif
