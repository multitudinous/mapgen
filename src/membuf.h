#ifndef MEMBUF_H
#define MEMBUF_H

#include <memory>

class MemBuf
{
public:
	MemBuf();
	MemBuf(int size, int lengthX, int lengthY=1);
	virtual ~MemBuf();

	void setSize(int size, int lengthX, int lengthY=1);
	void destroy();

	int getSize() const { return m_size; } // size of a single element in bytes
	int getLenX() const { return m_lenX; }
	int getLenY() const { return m_lenY; }
	int getByteCount() const { return m_size*m_lenX*m_lenY; }
	void* getBuf() { return m_pbuf; }
	const void* getBufRead() const { return m_pbuf; }

protected:
	void* m_pbuf;
	int m_size;
	int m_lenX;
	int m_lenY;
};

typedef std::shared_ptr<MemBuf> PMemBuf;

#endif
