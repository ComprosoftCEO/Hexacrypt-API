#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Hexacrypt.h>

int main(int argc, char** argv) {

    if (argc < 3) {return 1;}

    for (int i = 0; i < 32; ++i) {
        char* encoded = HexacryptPlus_Encrypt(argv[1], argv[2], 10);
        printf("\n********%s\n",encoded); fflush(stdout);

        char* decoded = HexacryptPlus_Decrypt(encoded,argv[2], 10);
        printf("\n********%s\n",decoded); fflush(stdout);

        free(encoded);
        free(decoded);
    }
}
