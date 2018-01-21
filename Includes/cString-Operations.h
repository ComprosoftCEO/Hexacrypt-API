#ifndef C_STRING_OPERATIONS_HEADER
#define C_STRING_OPERATIONS_HEADER

#include <HString.h>
#include <cRand64.h>


void Filter_Ascii(char* string, size_t len);
void pseudoXOR(char* string, pRand64 rand);
void Shuffle_String(pRand64 rand, char* string, size_t len);
void Reverse_String(char* string);


#endif // C_STRING_OPERATIONS_HEADER Included
