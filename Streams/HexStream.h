#ifndef HEX_STREAM_HEADER
#define HEX_STREAM_HEADER

#include <stdint.h>
#include <stdlib.h>

typedef void* pHexStream;


#define DEFAULT_STATE 64
pHexStream New_HexStream(const char* seed, uint32_t state);
void Free_HexStream(pHexStream stream);

void HexStream_Reset(pHexStream stream);
void HexStream_Reseed(pHexStream stream, const char* seed);

char HexStream_Next(pHexStream stream);
void HexStream_NextString(pHexStream stream, char* buf, size_t count);

uint32_t HexStream_GetInt(pHexStream stream);
uint64_t HexStream_GetLong(pHexStream stream);


#endif // HEX_STREAM_HEADER included
