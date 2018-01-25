#ifndef PRIME_MAP_HEADER
#define PRIME_MAP_HEADER

#include <stdlib.h>
#include <stdint.h>

typedef void* pPrimeMap;

pPrimeMap New_Prime_Map(const char* key, size_t len);
void Free_Prime_Map(pPrimeMap pmap);

uint32_t Prime_Map(pPrimeMap pmap, uint32_t input);
void Shuffle_Prime_Map(pPrimeMap pmap);



#endif // PRIME_MAP_HEADER Included
