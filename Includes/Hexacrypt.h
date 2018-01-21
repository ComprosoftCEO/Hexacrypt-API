#ifndef HEXACRYPT_HEADER
#define HEXACRYPT_HEADER

#include <stdint.h>

char* Hexacrypt_Encrypt(const char* plaintext, const char* key);
char* Hexacrypt_Decrypt(const char* ciphertext, const char* key);

char* HexacryptPlus_Encrypt(const char* plaintext, const char* key, uint32_t rounds);
char* HexacryptPlus_Decrypt(const char* ciphertext, const char* key, uint32_t rounds);


#endif // HEXACRYPT_HEADER
