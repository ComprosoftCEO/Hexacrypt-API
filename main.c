#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cHexStream.h>
#include <cMatrixCode.h>

int main(int argc, char** argv) {

    if (argc < 2) {return 1;}
    char* buf = malloc((strlen(argv[1])+1) * sizeof(char));

    pHexStream stream = New_HexStream(argv[1],10);
    while(1) {
        HexStream_Next(stream);
    }

    strcpy(buf,argv[1]);
    printf("%d\n",strlen(buf));

    //MatrixCode(buf,"Secret Key",DEFAULT_COMMANDS);
    //printf("%s\n",buf);
    //MatrixCode_Inverse(buf,"Secret Key",DEFAULT_COMMANDS);
    //printf("%s\n",buf);

}
