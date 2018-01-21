#include <HString.h>

pHString New_HString(int rounds, int garbage, size_t string_len) {

    //            Front/Back Garbage    Counters    The String itself
    size_t size = 2*(rounds * garbage) + 2*rounds + string_len;
    pHString hstr = (pHString) malloc(sizeof(HString));
    hstr->buf = calloc(size,sizeof(char));
    hstr->string = hstr->buf + (rounds*garbage);    //Offset by front garbage

    return hstr;
}


void Free_HString(pHString hstr) {
    free(hstr->buf);
    free(hstr);
}
