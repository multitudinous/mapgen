#ifndef TEXTURES_H
#define TEXTURES_H

#include "texture.h"

class Textures
{
public:

public:
	Textures();
	virtual ~Textures();

	int Count() { return (int)m_txs.size(); }
	void Insert(PTexture tx);
	bool Remove(PTexture tx);

	void Destroy();

	const PTexture& operator[](const int location) const;
	PTexture& operator[](const int location);
	const PTexture& operator[](const unsigned int location) const;
	PTexture& operator[](const unsigned int location);

protected:
	std::vector<PTexture> m_txs;
};

#endif
