//Main function for testing new features
#include <stdio.h>
#include <String-Operations.h>
#include <MathHash.h>


int main(int argc, char** argv) {

    pMathHash mh = New_MathHash(3,10);

    MathHash_Print(mh);

    Free_MathHash(mh);


    //const char* enc = HexacryptPlus_Encrypt("Hello","Key",10);
    //printf("%s\n",enc);
    //const char* dec = HexacryptPlus_Decrypt(enc,"Key",10);
    //printf("%s\n",dec);
}
