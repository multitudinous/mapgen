#include "textures.h"


Textures::Textures()
{
}

Textures::~Textures()
{
	Destroy();
}

void Textures::Insert(PTexture tx)
{
	m_txs.push_back(tx);
}

bool Textures::Remove(PTexture tx)
{
	std::vector<PTexture>::iterator it = m_txs.begin();
	 while (it != m_txs.end())
	 {
		 if (*it == tx)
		 {
			 m_txs.erase(it);
			 return true;
		 }
		 it++;
	 }
	 
	 return false;
}

void Textures::Destroy()
{
	m_txs.clear();
}

const PTexture& Textures::operator[](const int location) const
{    
	if (location < 0 || location >= (int)m_txs.size()) throw "Invalid array access"; 
	return m_txs[location];
}

PTexture& Textures::operator[](const int location)
{    
	if (location < 0 || location >= (int)m_txs.size()) throw "Invalid array access";    
	return m_txs[location];
}

const PTexture& Textures::operator[](const unsigned int location) const
{    
	if (location >= m_txs.size()) throw "Invalid array access"; 
	return m_txs[location];
}

PTexture& Textures::operator[](const unsigned int location)
{    
	if (location >= m_txs.size()) throw "Invalid array access";    
	return m_txs[location];
}