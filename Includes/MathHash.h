#ifndef MATH_HASH_HEADER
#define MATH_HASH_HEADER

#include <stdint.h>

//Math Hash starts with an integer constant, then feeds data
//  into the state until a flush is requested

typedef void* pMathHash;

pMathHash New_MathHash(uint32_t seed);
pMathHash MathHash_Reset(pMathHash, uint32_t seed);
void Free_MathHash(pMathHash);

void MathHash_NextRound(pMathHash, uint32_t data);
uint32_t MathHash_FlushInt(pMathHash);


#endif // MATH_HASH_HEADER Included
