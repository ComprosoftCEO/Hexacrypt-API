#ifndef C_HEX_STREAM_HEADER
#define C_HEX_STREAM_HEADER

#include <stdint.h>
#include <stdlib.h>

typedef void* pHexStream;


#define DEFAULT_STATE 64
pHexStream New_HexStream(const char* seed, uint32_t state);
void Free_HexStream(pHexStream stream);

void HexStream_Reset(pHexStream stream);

char HexStream_Next(pHexStream stream);
char* HexStream_NextString(pHexStream stream, size_t count);

uint32_t HexStream_GetInt(pHexStream stream);
uint64_t HexStream_GetLong(pHexStream stream);


#endif // C_HEX_STREAM_HEADER included
