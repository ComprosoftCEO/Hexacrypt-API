#ifndef C_MATRIX_CODE_HEADER
#define C_MATRIX_CODE_HEADER

#define DEFAULT_COMMANDS 24

#include <cStringMatrix.h>
#include <stdint.h>

void MatrixCode(char* string, const char* key, uint32_t commands);
void MatrixCode_Inverse(char* string, const char* key, uint32_t commands);




#endif // C_MATRIX_CODE_HEADER included
