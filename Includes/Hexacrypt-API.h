#ifndef HEXACRYPT_API_HEADER
#define HEXACRYPT_API_HEADER

#include <Dll.h>
#include <stdint.h>

#define BUF_SIZE 32769
extern char output_buffer[];

DLL_EXPORT const char* Hexacrypt_Encrypt(const char* plaintext, const char* key);
DLL_EXPORT const char* Hexacrypt_Decrypt(const char* ciphertext, const char* key);

DLL_EXPORT const char* HexacryptPlus_Encrypt(const char* plaintext, const char* key, uint32_t rounds);
DLL_EXPORT const char* HexacryptPlus_Decrypt(const char* ciphertext, const char* key, uint32_t rounds);


#endif // HEXACRYPT_API_HEADER Included
