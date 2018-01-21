#include <HString.h>
#include <string.h>

//Private HString object
typedef struct {
    char* str;
    char* buf;      // Address of the allocated buffer
} HString_Obj, *pHString_Obj;


pHString New_HString(const char* string, unsigned int rounds, unsigned int garbage) {

    size_t string_len = strlen(string);

    //            Front/Back Garbage    Counters   The String itself   Null Terminator
    size_t size = 2*(rounds * garbage) + 2*rounds + string_len +         1;
    pHString_Obj hstr = (pHString_Obj) malloc(sizeof(HString));
    hstr->buf = calloc(size,sizeof(char));
    hstr->str = hstr->buf + (rounds*garbage);    //Offset by front garbage

    strcpy(hstr->str,string);

    return (pHString) hstr;
}


void Free_HString(pHString hstr) {
    free(((pHString_Obj) hstr)->buf);
    free(hstr);
}
