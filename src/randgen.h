//=====================================================
// File name: randgen.h
// Authors: Brian Bailey 
// Created: 02-12-2000
// Revised: 02-17-2000
// Description: class CRandom
// Contents:  class header for random number methods
//======================================================
#ifndef RANDGEN_H
#define RANDGEN_H

class RandGen
{

public:

	// Construction/Destruction ///////////////////////////////
	RandGen(){ seed(); };

	// Methods ////////////////////////////////////////////////
	float genRandPercent(int NumPossibilities=1000);		// generate a random percent (between 0 - 1) 
	int   genRandInt(int low, int high);			// generate a random integer 
	int   genRandInt2(int low, int high);			// generate a random integer (more random)
	void  seed(void);			                    // seeds the random number generator								
};

#endif