#ifndef C_STRING_MATRIX_HEADER
#define C_STRING_MATRIX_HEADER

#include <stdint.h>

typedef void* pStringMatrix;


//Due to the way function pointers work, use a single structure
//  so all "opcodes" in the string matrix have the same id
#define DO_NORMAL  0
#define DO_INVERSE 1
typedef struct MC_PARAMS {
    uint64_t param[4];
    uint64_t doInverse;
} StringMatrix_Params, *pStringMatrix_Params;


pStringMatrix New_StringMatrix(char* string);
void Free_StringMatrix(pStringMatrix matrix);

void StringMatrix_Print(pStringMatrix matrix);

void StringMatrix_CellSwap(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_CellRowSwap(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_CellColSwap(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_RowSwap(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_ColSwap(pStringMatrix matrix, pStringMatrix_Params params);

void StringMatrix_RowReverse(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_ColReverse(pStringMatrix matrix, pStringMatrix_Params params);

void StringMatrix_RowRotate(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_ColRotate(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_LocalSwap(pStringMatrix matrix, pStringMatrix_Params params);

//Move all rows a given direction
void StringMatrix_ShiftRows(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_ShiftCols(pStringMatrix matrix, pStringMatrix_Params params);

void StringMatrix_PseudoXOR_Cell(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_PseudoXOR_Row(pStringMatrix matrix, pStringMatrix_Params params);
void StringMatrix_PseudoXOR_Col(pStringMatrix matrix, pStringMatrix_Params params);

#endif // C_STRING_MATRIX_HEADER Included
