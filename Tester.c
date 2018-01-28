//Main function for testing new features
#include <stdio.h>
#include <String-Operations.h>
#include <PrimeMap.h>

int main(int argc, char** argv) {

    pPrimeMap pmap = New_Prime_Map(1);
    uint64_t result, temp;
    for (result = 0; result < ((uint64_t) 1 << 32); ++result) {
        printf("Searching For: %u -> ", (uint32_t) result);
        for (temp = 0; temp < ((uint64_t) 1 << 32); ++temp) {
            uint32_t out = Prime_Map(pmap,(uint32_t) temp);
            if (out == (uint32_t) result) {
                printf("%u (%u)\n",(uint32_t) temp, (uint32_t) out);
                goto next;
            }
        }
        printf("Not Found!\n");

        next: continue;
    }

    //const char* enc = HexacryptPlus_Encrypt("Hello","Key",10);
    //printf("%s\n",enc);
    //const char* dec = HexacryptPlus_Decrypt(enc,"Key",10);
    //printf("%s\n",dec);
}
