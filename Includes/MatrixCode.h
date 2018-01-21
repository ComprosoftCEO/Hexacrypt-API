#ifndef MATRIX_CODE_HEADER
#define MATRIX_CODE_HEADER

#define DEFAULT_COMMANDS 24

#include <StringMatrix.h>
#include <stdint.h>

void MatrixCode(char* string, const char* key, uint32_t commands);
void MatrixCode_Inverse(char* string, const char* key, uint32_t commands);




#endif // MATRIX_CODE_HEADER included
