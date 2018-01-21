#include <StringMatrix.h>
#include <String-Operations.h>
#include <Rand64.h>

#include <stdio.h>
#include <string.h>


typedef struct {
    char* state;
    uint64_t rows;      // Total number of rows in this matrix
    uint64_t cols;      // Total number of columns in this matrix
} StringMatrix_Obj, *pStringMatrix_Obj;

#define MATRIX(mat,row,col) (mat->state[(row) * mat->cols + (col)])
#define SWAP(type,a,b) \
    type TEMP = (a); \
    (a) = (b); \
    (b) = (TEMP);

pStringMatrix New_StringMatrix(char* string) {
    pStringMatrix matrix = (pStringMatrix) malloc(sizeof(StringMatrix_Obj));
    StringMatrix_SetString(matrix,string);
    return matrix;
}

void Free_StringMatrix(pStringMatrix matrix) {
    free(matrix);
}


//Recalculate da params
void StringMatrix_SetString(pStringMatrix matrix, char* string) {

    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;

    //Figure out the number of rows and cols
    uint64_t len = strlen(string);
    mobj->state = string;

    //Default values
    mobj->rows = 1;
    mobj->cols = len;

     //Default perimeter, absolute worst case scenario
    uint64_t temp_row, temp_col;
    uint64_t temp_perimeter, perimeter = 2+ 2*mobj->cols;

    //Find the row and column count that:
    //  1. Minimizes the perimeter
    //  2. Picks the smallest row count (4x6 instead of 6x4)
    for (temp_row = 2; temp_row <= ((len / 2) + (len % 2)); ++temp_row) {

        if ((len % temp_row) == 0) {

            temp_col = len / temp_row;
            temp_perimeter = (2*temp_row) + (2*temp_col);

            if (temp_perimeter < perimeter) {
                perimeter = temp_perimeter;
                mobj->rows = temp_row;
                mobj->cols = temp_col;
            }
        }
    }
}


void StringMatrix_Print(pStringMatrix matrix) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;

    printf("%llu x %llu\n",mobj->rows,mobj->cols);
    printf("===============\n");
    for (row = 0; row < mobj->rows; ++row) {
     for (col = 0; col < mobj->cols; ++col) {
        printf("%c ",MATRIX(mobj,row,col));
     }  printf("\n");
    }
}



//Row1, Col1, Row2, Col2
void StringMatrix_CellSwap(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row1, col1, row2, col2;

    if (mobj->rows && mobj->cols) {

        //Parse the params
        row1 = params->param[0] % mobj->rows;
        col1 = params->param[1] % mobj->cols;
        row2 = params->param[2] % mobj->rows;
        col2 = params->param[3] % mobj->cols;

        SWAP(char,
             MATRIX(mobj,row1,col1),
             MATRIX(mobj,row2,col2)
        );
    }
}

//Row,Col1,Col2
void StringMatrix_CellRowSwap(pStringMatrix matrix, pStringMatrix_Params params) {
    params->param[3] = params->param[2];
    params->param[2] = params->param[0];
    StringMatrix_CellSwap(matrix,params);
}

//Row1,Col,Row2
void StringMatrix_CellColSwap(pStringMatrix matrix, pStringMatrix_Params params) {
    params->param[3] = params->param[1];
    StringMatrix_CellSwap(matrix,params);
}

//Row1, Row2
void StringMatrix_RowSwap(pStringMatrix matrix, pStringMatrix_Params params) {
    uint64_t col, row1, row2;
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;

    if (mobj->rows) {
        row1 = params->param[0] % mobj->rows;
        row2 = params->param[1] % mobj->rows;

        for (col = 0; col < mobj->cols; ++col) {
            SWAP(char,
                 MATRIX(mobj,row1,col),
                 MATRIX(mobj,row2,col)
            );
        }
    }
}

//Col1, Col2
void StringMatrix_ColSwap(pStringMatrix matrix, pStringMatrix_Params params) {
    uint64_t row, col1, col2;
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;

    if (mobj->cols) {
        col1 = params->param[0] % mobj->cols;
        col2 = params->param[1] % mobj->cols;

        for (row = 0; row < mobj->rows; ++row) {
            SWAP(char,
                 MATRIX(mobj,row,col1),
                 MATRIX(mobj,row,col2)
            );
        }
    }
}



//Row
void StringMatrix_RowReverse(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;

    if (mobj->rows) {
        row = params->param[0] % mobj->rows;

        for (col = 0; col < (mobj->cols/2); ++col) {
            SWAP(char,
                 MATRIX(mobj,row,col),
                 MATRIX(mobj,row,(mobj->cols -1) - col)
            );
        }
    }
}

//Col
void StringMatrix_ColReverse(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;

    if (mobj->cols) {
        col = params->param[0] % mobj->cols;

        for (row = 0; row < (mobj->rows/2); ++row) {
            SWAP(char,
                 MATRIX(mobj,row,col),
                 MATRIX(mobj,(mobj->rows - 1) - row,col)
            );
        }
    }
}



//Row, [0 = left, 1 = right]
void StringMatrix_RowRotate(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col, dir;
    char temp;

    if (mobj->rows) {
        row = params->param[0] % mobj->rows;
        dir = (params->param[1] & 1) ^ params->doInverse;


        //Rotate Left
        if (dir == 0) {
            temp = MATRIX(mobj,row,0);
            for (col = 1; col < mobj->cols; ++col) {
                MATRIX(mobj,row,col-1) = MATRIX(mobj,row,col);
            }
            MATRIX(mobj,row,mobj->cols - 1) = temp;


        //Rotate Right
        } else {
            temp = MATRIX(mobj,row,mobj->cols - 1);
            for (col = mobj->cols; col > 1; --col) {
                MATRIX(mobj,row,col-1) = MATRIX(mobj,row,col-2);
            }
            MATRIX(mobj,row,0) = temp;
        }
    }
}


//Col, [0 = Up, 1 = Down]
void StringMatrix_ColRotate(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col, dir;
    char temp;

    if (mobj->cols) {
        col = params->param[0] % mobj->cols;
        dir = (params->param[1] & 1) ^ params->doInverse;


        //Rotate Up
        if (dir == 0) {
            temp = MATRIX(mobj,0,col);
            for (row = 1; row < mobj->rows; ++row) {
                MATRIX(mobj,row-1,col) = MATRIX(mobj,row,col);
            }
            MATRIX(mobj,mobj->rows-1,col) = temp;


        //Rotate Down
        } else {
            temp = MATRIX(mobj,mobj->rows - 1,col);
            for (row = mobj->rows; row > 1; --row) {
                MATRIX(mobj,row-1,col) = MATRIX(mobj,row-2,col);
            }
            MATRIX(mobj,0,col) = temp;
        }
    }
}


//Row, Col, [0 = up, 1 = down, 2 = left, 3 = right]
void StringMatrix_LocalSwap(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col, row1, col1, dir;

    if (mobj->rows && mobj->cols) {
        row = params->param[0] % mobj->rows;
        col = params->param[1] % mobj->cols;
        dir = (params->param[2] & 3);

        row1 = row;
        col1 = col;

        switch(dir) {
        case 0: //Up
            row1 = (row1 - 1) % mobj->rows; break;
        case 1: //Down
            row1 = (row1 + 1) % mobj->rows; break;
        case 2: //Left
            col1 = (col1 - 1) % mobj->cols; break;
        case 3: //Right
            col1 = (col1 + 1) % mobj->cols; break;
        }

        SWAP(char,
             MATRIX(mobj,row,col),
             MATRIX(mobj,row1,col1)
        );
    }
}




//[0 = Rows Up, 1 = Rows Down]
void StringMatrix_ShiftRows(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t col;

    if (mobj->rows) {

        //Shift every col up by 1
        params->param[1] = params->param[0];
        for (col = 0; col < mobj->cols; ++col) {
            params->param[0] = col;
            StringMatrix_ColRotate(matrix,params);
        }
    }
}

//[0 = Cols Left, 1 = Cols Right]
void StringMatrix_ShiftCols(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row;

    if (mobj->rows) {

        //Shift every row by 1
        params->param[1] = params->param[0];
        for (row = 0; row < mobj->rows; ++row) {
            params->param[0] = row;
            StringMatrix_RowRotate(matrix,params);
        }
    }
}



//Row,Col,Seed
void StringMatrix_PseudoXOR_Cell(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row,col;
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}
    if (mobj->rows && mobj->cols) {

        row = params->param[0] % mobj->rows;
        col = params->param[1] % mobj->cols;

        Rand64_Reseed(rand,params->param[2]);
        pseudoXOR_NChar(mobj->state + (row*mobj->cols) + col,1,1,rand);
    }
}

//Row,Seed
void StringMatrix_PseudoXOR_Row(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row;
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}
    if (mobj->rows) {
        row = params->param[0] % mobj->rows;

        Rand64_Reseed(rand,params->param[1]);
        pseudoXOR_Length(mobj->state+(row * mobj->cols),mobj->cols,rand);

    }
}


//Col,Seed
void StringMatrix_PseudoXOR_Col(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t col;
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}
    if (mobj->cols) {
        col = params->param[0] % mobj->cols;

        Rand64_Reseed(rand,params->param[1]);
        pseudoXOR_NChar(mobj->state+col,mobj->cols,mobj->rows,rand);
    }
}
