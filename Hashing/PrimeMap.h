#ifndef PRIME_MAP_HEADER
#define PRIME_MAP_HEADER

#include <stdlib.h>
#include <stdint.h>

typedef void* pPrimeMap;

pPrimeMap New_Prime_Map(uint64_t seed);
void Reseed_Prime_Map(pPrimeMap pm, uint64_t seed);
void Free_Prime_Map(pPrimeMap pmap);

uint32_t Prime_Map(pPrimeMap pmap, uint32_t input);
void Shuffle_Prime_Map(pPrimeMap pmap);



#endif // PRIME_MAP_HEADER Included
