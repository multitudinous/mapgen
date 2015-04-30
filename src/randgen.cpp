//=====================================================
// File name: RandGen.cpp
// Authors: Brian Bailey 
// Created: 02-12-2000
// Revised: 02-17-2000
// Description: class RandGen
// Contents:  class methods for random number generation
//======================================================
#include "randgen.h"
#include <stdlib.h> 
#include <time.h> 



////////////////////////////////////////////////////////////////////////////////
// GenRandPercent: Generates a ramdom number between 0-1 inclusively
//
// NumPossibilities: Number of floating point possibilities between 0-1
// return:           a floating point number between 0-1 - randomly generated
////////////////////////////////////////////////////////////////////////////////
float RandGen::genRandPercent(int NumPossibilities)
{
	int  random;

	random = genRandInt2(0, NumPossibilities);  // using GenRandInt2 - seems to produce better results

	return ( (float)random/(float)NumPossibilities );

}


////////////////////////////////////////////////////////////////////////////////
// GenRandInt: guarantees that the returned value is 
//						 between low and high inclusively    
// 
// low:   lowest value possible
// high:  highest value possible
// return: the randomly generated value 
////////////////////////////////////////////////////////////////////////////////
int RandGen::genRandInt(int low, int high)
{
  return (rand() % (high - low + 1) + low);
}


////////////////////////////////////////////////////////////////////////////////
// GenRandInt2: guarantees that the returned value is 
//						   between low and high inclusively  
//              Seems to be a little better than GenRandInt  
// 
// low:   lowest value possible
// high:  highest value possible
// return: the randomly generated value 
////////////////////////////////////////////////////////////////////////////////
int RandGen::genRandInt2(int low, int high)
{
  return  (low+ (int) ((float)(high)*rand()/(RAND_MAX+(float)low)));
}

////////////////////////////////////////////////////////////////////////////////
// Seed: seeds the random number generator with the system time
////////////////////////////////////////////////////////////////////////////////
void RandGen::seed(void)
{
	srand((unsigned)time((time_t *) NULL));
}