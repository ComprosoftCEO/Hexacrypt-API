#include <cMatrixCode.h>

#include <stdio.h>
#include <cStringMatrix.h>
#include <cHexStream.h>

//Used to aid inverse function
typedef struct {
    StringMatrix_Params params;
    unsigned char command;
} MatrixCode_Command;

static StringMatrix_Params params;


typedef void (*MatrixCode_Function)(pStringMatrix,pStringMatrix_Params);

static const int FUNCTION_COUNT = 15;
static const MatrixCode_Function MatrixCode_Functions[] = {
    StringMatrix_CellSwap,
    StringMatrix_CellRowSwap,
    StringMatrix_CellColSwap,
    StringMatrix_RowSwap,
    StringMatrix_ColSwap,
    StringMatrix_RowReverse,
    StringMatrix_ColReverse,
    StringMatrix_RowRotate,
    StringMatrix_ColRotate,
    StringMatrix_LocalSwap,
    StringMatrix_ShiftRows,
    StringMatrix_ShiftCols,
    StringMatrix_PseudoXOR_Cell,
    StringMatrix_PseudoXOR_Row,
    StringMatrix_PseudoXOR_Col,
};

static const unsigned int paramsNeeded[] = {
    4,3,3,2,2,1,1,2,2,3,1,1,3,2,2
};

void MatrixCode(char* string, const char* key, uint32_t commands) {

    uint32_t i,j;
    pStringMatrix matrix = New_StringMatrix(string);
    pHexStream hstream = New_HexStream(key,DEFAULT_STATE);
    params.doInverse = DO_NORMAL;

    for (i = 0; i < commands; ++i) {

        unsigned char command = (HexStream_Next(hstream) % (FUNCTION_COUNT + 1));
        //printf("%d\n",command);
        //fflush(stdout);

        //printf("%d: ",command);
        if (command) {
            --command;

            for (j = 0; j < paramsNeeded[command]; ++j) {
                params.param[j] = HexStream_GetLong(hstream);
                //printf("%lu,",params.param[j]);
            } //printf("\n");

            MatrixCode_Functions[command](matrix,&params);
        }

        printf("%d\t%s\n",command+1,string);
    }

   //Free_HexStream(hstream);
   //Free_StringMatrix(matrix);
}



void MatrixCode_Inverse(char* string, const char* key, uint32_t commands) {

    pHexStream hstream = New_HexStream(key,DEFAULT_STATE);
    pStringMatrix matrix = New_StringMatrix(string);
    MatrixCode_Command *com, *allCommands = malloc(commands*sizeof(MatrixCode_Command));
    uint32_t i,j;



    //Step 1: Get All Commands
    for (i = 0; i < commands; ++i) {
        com = allCommands+i;
        com->command = (HexStream_Next(hstream) % (FUNCTION_COUNT + 1));
        com->params.doInverse = DO_INVERSE;
        if (com->command) {
            unsigned char temp_command = com->command - 1;
            for (j = 0; j < paramsNeeded[temp_command]; ++j) {
                com->params.param[j] = HexStream_GetLong(hstream);
            }
        }
    }

    //Step 2: Run Through All Commands in Reverse
    for (i = commands; i > 0; --i) {
        com = allCommands + (i - 1);
        printf("%d\t%s\n",com->command,string);
        if (com->command) {
            MatrixCode_Functions[com->command - 1](matrix,&com->params);
        }
    }

    //Free_HexStream(hstream);
    Free_StringMatrix(matrix);
    free(allCommands);
}
