#ifndef MATH_HASH_HEADER
#define MATH_HASH_HEADER

#include <stdint.h>

//Math Hash starts with an integer constant, then feeds data
//  into the state until a flush is requested
typedef void* pMathHash;

//  size  = Length of the side of a chunk (size*size*size)
//  count = Total number of chunks
pMathHash New_MathHash(uint8_t side, uint16_t count);
void Free_MathHash(pMathHash);
void MathHash_Print(pMathHash);


void MathHash_Absorb(pMathHash, uint32_t data);
uint32_t MathHash_Squeeze(pMathHash);


#endif // MATH_HASH_HEADER Included
