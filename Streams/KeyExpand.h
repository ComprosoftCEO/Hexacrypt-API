#ifndef KEY_EXPAND_HEADER
#define KEY_EXPAND_HEADER

#include <HexStream.h>
#include <Rand64.h>

typedef struct {
    char* str;            // Buffer that holds the key
    pRand64 counter;      // How many chars inside the next key???
    pHexStream stream;    // Used to generate subsequent keys
} KeyExpand, *pKeyExpand;

//Auto filters the string before anything else
pKeyExpand New_KeyExpander(const char* str);
void Free_KeyExpander(pKeyExpand ke);

void ExpandKey(pKeyExpand ke);


#endif // KEY_EXPAND_HEADER included
