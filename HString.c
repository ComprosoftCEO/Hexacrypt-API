#include <HString.h>
#include <cString-Operations.h>
#include <string.h>

//Private HString object
typedef struct {
    char* str;
    char* buf;      // Address of the allocated buffer
} HString_Obj, *pHString_Obj;


pHString New_HString(const char* string, unsigned int rounds,
                      unsigned int front_garbage, unsigned int back_garbage) {

    size_t string_len = strlen(string);

    //                 Front Back Garbage                  Counters   String    Null Terminator
    size_t size = (front_garbage + back_garbage)*rounds + 2*rounds + string_len +  1;
    pHString_Obj hstr = (pHString_Obj) malloc(sizeof(HString));
    hstr->buf = calloc(size,sizeof(char));
    hstr->str = hstr->buf + (rounds*front_garbage);    //Offset by front garbage

    strcpy(hstr->str,string);

    Filter_Ascii(hstr->str,string_len);

    return (pHString) hstr;
}


void Free_HString(pHString hstr) {
    free(((pHString_Obj) hstr)->buf);
    free(hstr);
}
