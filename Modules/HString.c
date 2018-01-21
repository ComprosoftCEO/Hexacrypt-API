#include <HString.h>
#include <String-Operations.h>
#include <string.h>

//Private HString object
typedef struct {
    char* str;
    char* buf;      // Address of the allocated buffer
} HString_Obj, *pHString_Obj;


pHString New_HString(const char* string, size_t maxlen, unsigned int rounds,
                      unsigned int front_garbage, unsigned int back_garbage) {


    //If maxlen is greater than string length, then only copy
    size_t string_len = strlen(string);
    size_t front = front_garbage*rounds, back = back_garbage*rounds;
    size_t counters = rounds*2;

    //Yeah, this is BAD
    if ((front+back+counters+1) > maxlen) {return NULL;}

    //How much to round off???
    maxlen-=(front+back+counters+1);
    if (string_len > maxlen) {string_len = maxlen;}

    size_t size = front + back + counters + string_len +  1;
    pHString_Obj hstr = (pHString_Obj) malloc(sizeof(HString));

    hstr->buf = calloc(size,sizeof(char));
    hstr->str = hstr->buf + front;    //Offset by front garbage
    strncpy(hstr->str,string,string_len);

    Filter_Ascii(hstr->str,string_len);

    return (pHString) hstr;
}


void Free_HString(pHString hstr) {
    free(((pHString_Obj) hstr)->buf);
    free(hstr);
}
