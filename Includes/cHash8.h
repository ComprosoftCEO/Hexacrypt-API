#ifndef C_HASH_8_HEADER
#define C_HASH_8_HEADER

#include <stdlib.h>
#include <stdint.h>

#define HASH8_FUNCTION(name,type) type Hash8_##name(const char* str)
#define HASH8_LENGTH_FUNCTION(name,type) type Hash8_##name##_Length(const char* str, size_t len)

HASH8_FUNCTION(U8,uint8_t);
HASH8_FUNCTION(U16,uint16_t);
HASH8_FUNCTION(U32,uint32_t);
HASH8_FUNCTION(U64,uint64_t);

HASH8_LENGTH_FUNCTION(U8,uint8_t);
HASH8_LENGTH_FUNCTION(U16,uint16_t);
HASH8_LENGTH_FUNCTION(U32,uint32_t);
HASH8_LENGTH_FUNCTION(U64,uint64_t);


#endif // C_HASH_8_HEADER Included
