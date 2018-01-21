#include <cStringMatrix.h>
#include <cString-Operations.h>
#include <cRand64.h>

#include <stdio.h>
#include <string.h>


typedef struct {
    char* state;
    uint64_t rows;      // Total number of rows in this matrix
    uint64_t cols;      // Total number of columns in this matrix
    char* row_buf;      // Temporary buffer used when doing pseudoXOR on a row
    char* col_buf;      // Temporary buffer used when doing pseudoXOR on a col
} StringMatrix_Obj, *pStringMatrix_Obj;

#define MATRIX(mat,row,col) (mat->state[(row) * mat->cols + (col)])
#define SWAP(type,a,b) \
    type TEMP = (a); \
    (a) = (b); \
    (b) = (TEMP);

pStringMatrix New_StringMatrix(char* string) {

    printf("%s\n",string);

    pStringMatrix_Obj matrix = malloc(sizeof(StringMatrix_Obj));

    //Figure out the number of rows and cols
    uint64_t len = strlen(string);
    matrix->state = string;

    //Default values
    matrix->rows = 1;
    matrix->cols = len;

     //Default perimeter, absolute worst case scenario
    uint64_t temp_row, temp_col;
    uint64_t temp_perimeter, perimeter = 2+ 2*matrix->cols;

    //Find the row and column count that:
    //  1. Minimizes the perimeter
    //  2. Picks the smallest row count (4x6 instead of 6x4)
    for (temp_row = 2; temp_row <= ((len / 2) + (len % 2)); ++temp_row) {

        if ((len % temp_row) == 0) {

            temp_col = len / temp_row;
            temp_perimeter = (2*temp_row) + (2*temp_col);

            if (temp_perimeter < perimeter) {
                perimeter = temp_perimeter;
                matrix->rows = temp_row;
                matrix->cols = temp_col;
            }
        }
    }

    matrix->row_buf = calloc(matrix->cols + 1, sizeof(char));
    matrix->col_buf = calloc(matrix->rows + 1, sizeof(char));

    return (pStringMatrix) matrix;
}

void Free_StringMatrix(pStringMatrix matrix) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;

    free(mobj->row_buf);
    free(mobj->col_buf);
    free(matrix);
}


void StringMatrix_Print(pStringMatrix matrix) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;

    for (row = 0; row < mobj->rows; ++row) {
     for (col = 0; col < mobj->cols; ++col) {
        printf("%c ",MATRIX(mobj,row,col));
     }  printf("\n");
    }
    fflush(stdout);
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

        #ifdef DEBUG
        printf("Cell Swap: (%llu,%llu) (%llu,%llu)\n",row1,col1,row2,col2); fflush(stdout);
        #endif
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

        #ifdef DEBUG
        printf("Row Swap: %llu %llu\n",row1,row2); fflush(stdout);
        #endif

        for (col = 0; col < mobj->cols; ++col) {
            SWAP(char,
                 MATRIX(mobj,row1,col),
                 MATRIX(mobj,row2,col)
            );

            #ifdef EXTRA_DEBUG
            printf("\\-Cell Swap: (%llu %llu) (%llu %llu)\n",row1,col,row2,col); fflush(stdout);
            #endif
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

        #ifdef DEBUG
        printf("Col Swap: %llu %llu\n",col1,col2); fflush(stdout);
        #endif

        for (row = 0; row < mobj->rows; ++row) {
            SWAP(char,
                 MATRIX(mobj,row,col1),
                 MATRIX(mobj,row,col2)
            );

            #ifdef EXTRA_DEBUG
            printf("\\-Cell Swap: (%llu,%llu) (%llu,%llu)\n",row,col1,row,col2); fflush(stdout);
            #endif
        }
    }
}



//Row
void StringMatrix_RowReverse(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;

    if (mobj->rows) {
        row = params->param[0] % mobj->rows;

        #ifdef DEBUG
        printf("Row Reverse: %llu\n",row); fflush(stdout);
        #endif

        for (col = 0; col < (mobj->cols/2); ++col) {
            SWAP(char,
                 MATRIX(mobj,row,col),
                 MATRIX(mobj,row,(mobj->cols -1) - col)
            );

            #ifdef EXTRA_DEBUG
            printf("\\-Cell Swap: (%llu,%llu) (%llu,%llu)\n",row,col,row,(mobj->cols -1) - col); fflush(stdout);
            #endif
        }

    }
}

//Col
void StringMatrix_ColReverse(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;

    if (mobj->cols) {
        col = params->param[0] % mobj->cols;

        #ifdef DEBUG
        printf("\\-Col Reverse: %llu\n",col); fflush(stdout);
        #endif
        for (row = 0; row < (mobj->rows/2); ++row) {
            SWAP(char,
                 MATRIX(mobj,row,col),
                 MATRIX(mobj,(mobj->rows - 1) - row,col)
            );

            #ifdef EXTRA_DEBUG
            printf("\\-Cell Swap: (%llu,%llu) (%llu,%llu)\n",row,col,(mobj->rows - 1) - row,col); fflush(stdout);
            #endif
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

        #ifdef DEBUG
        printf("Row Rotate: %llu [%d]\n",row,dir); fflush(stdout);
        #endif

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


        #ifdef DEBUG
        printf("Col Rotate: %llu [%d]\n",col,dir); fflush(stdout);
        #endif

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
    static uint64_t all_dir[] = {0,1,2,3,1,0,3,2};

    if (mobj->rows && mobj->cols) {
        row = params->param[0] % mobj->rows;
        col = params->param[1] % mobj->cols;
        dir = all_dir[(params->param[2] & 3) | (params->doInverse << 2)];

        #ifdef DEBUG
        printf("Local Swap: (%llu,%llu) [%d]\n",row,col,dir); fflush(stdout);
        #endif

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
    uint64_t row, col;
    static pRand64 rand = NULL;
    static char buf[2] = {0,0};

    if (!rand) {rand = New_Rand64_Seed(0);}
    if (mobj->rows && mobj->cols) {

        row = params->param[0] % mobj->rows;
        col = params->param[1] % mobj->cols;

        #ifdef DEBUG
        printf("PXOR Cell: (%llu,%llu)\n",row,col); fflush(stdout);
        #endif

        Rand64_Reseed(rand,params->param[2]);
        buf[0] = MATRIX(mobj,row,col);

        pseudoXOR(buf,rand);

        MATRIX(mobj,row,col) = buf[0];
    }
}

//Row,Seed
void StringMatrix_PseudoXOR_Row(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}
    if (mobj->rows) {
        row = params->param[0] % mobj->rows;

        #ifdef DEBUG
        printf("PXOR Row: %llu\n",row); fflush(stdout);
        #endif

        for (col = 0; col < mobj->cols; ++col) {
            mobj->row_buf[col] = MATRIX(mobj,row,col);
        }

        Rand64_Reseed(rand,params->param[1]);
        pseudoXOR(mobj->row_buf,rand);

        for (col = 0; col < mobj->cols; ++col) {
            MATRIX(mobj,row,col) = mobj->row_buf[col];
        }
    }
}


//Col,Seed
void StringMatrix_PseudoXOR_Col(pStringMatrix matrix, pStringMatrix_Params params) {
    pStringMatrix_Obj mobj = (pStringMatrix_Obj) matrix;
    uint64_t row, col;
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}
    if (mobj->cols) {
        col = params->param[0] % mobj->cols;

        #ifdef DEBUG
        printf("PXOR Col: %llu\n",col); fflush(stdout);
        #endif

        for (row = 0; row < mobj->rows; ++row) {
            mobj->col_buf[row] = MATRIX(mobj,row,col);
        }

        Rand64_Reseed(rand,params->param[1]);
        pseudoXOR(mobj->col_buf,rand);


        for (row = 0; row < mobj->rows; ++row) {
            MATRIX(mobj,row,col) = mobj->col_buf[row];
        }
    }
}
