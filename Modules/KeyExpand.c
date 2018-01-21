#include <KeyExpand.h>
#include <Hash8.h>
#include <String-Operations.h>
#include <stdlib.h>
#include <string.h>

pKeyExpand New_KeyExpander(const char* str) {

    pKeyExpand ke = (pKeyExpand) malloc(sizeof(KeyExpand));

    //Be sure to add the null-terminator
    size_t len, slen = strlen(str);
    if (slen < 255) {len = 256;} else {len = slen+1;}

    ke->str = calloc(len,sizeof(char));
    strcpy(ke->str,str);

    Filter_Ascii(ke->str,slen);

    ke->stream = New_HexStream(ke->str,128);
    ke->counter = New_Rand64_Seed(Hash8_U64(ke->str));

    return ke;
}

void Free_KeyExpander(pKeyExpand ke) {
    Free_HexStream(ke->stream);
    Free_Rand64(ke->counter);
    free(ke->str);
    free(ke);
}

void ExpandKey(pKeyExpand ke) {

    //Next key is a min of 0 and a max of 255
    uint8_t i, count = (uint8_t) (Rand64_Next(ke->counter) & 0xff);
    static char temp_buf[256];

    for (i = 0; i < count; ++i) {
        ke->str[i] = HexStream_Next(ke->stream);
    }
    ke->str[i] = 0; //Null Terminator

    //New state is the key reversed
    strcpy(temp_buf,ke->str);
    Reverse_String(temp_buf);
    HexStream_Reseed(ke->stream,temp_buf);
}
