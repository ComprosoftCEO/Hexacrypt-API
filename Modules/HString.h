//========================================================================
//  Hexacrypt String
//========================================================================
//  To speed up the process, char* in Hexacrypt is given largest possible
//  space to work with. This means adding space before and after the
//  character when first allocated to avoid reallocs, frees, etc. that slow
//  down the process
//
// [(Free Space)]  [A][B][C][D][0] [(Free Space)]
//     ^            ^
//  Buffer        String
#ifndef H_STRING_HEADER
#define H_STRING_HEADER

#include <stdlib.h>

typedef struct {
    char* str;      // The actual string being worked on
} HString, *pHString;


//Auto filters the string
//  maxlen = Maximum size of full buffer, INCLUDING null terminator!!!!!
pHString New_HString(const char* string, size_t maxlen,
                     unsigned int rounds, unsigned int front_garbage, unsigned int back_garbage);
void Free_HString(pHString hstr);



#endif // H_STRING Header included
