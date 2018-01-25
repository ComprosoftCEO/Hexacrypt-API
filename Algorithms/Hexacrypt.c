//=============================================
// Classic Hexacrypt Encryption Algorithm
//=============================================
#include <Hexacrypt-API.h>

#include <Hash8.h>
#include <Rand64.h>
#include <String-Operations.h>

#include <stdlib.h>
#include <string.h>

DLL_EXPORT const char* Hexacrypt_Encrypt(const char* plaintext, const char* key) {

    pHString hs_plaintext = New_HString(plaintext,BUF_SIZE,1,10,10);
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}

    Rand64_Reseed(rand,Hash8_U64(NULL,key));
    pseudoXOR(hs_plaintext->str,rand);

    Rand64_Reset(rand);
    Reverse_String(hs_plaintext->str);
    Add_Garbage(hs_plaintext,rand,1,9,0,10);

    Rand64_Reset(rand);
    pseudoXOR(hs_plaintext->str,rand);

    strncpy(output_buffer,hs_plaintext->str,BUF_SIZE - 1);

    Free_HString(hs_plaintext);

    return output_buffer;
}




DLL_EXPORT const char* Hexacrypt_Decrypt(const char* ciphertext, const char* key) {

    pHString hs_ciphertext = New_HString(ciphertext,BUF_SIZE,0,0,0);   //No extra space needed
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}

    Rand64_Reseed(rand,Hash8_U64(NULL,key));
    pseudoXOR(hs_ciphertext->str,rand);

    Rand64_Reset(rand);
    Remove_Garbage(hs_ciphertext,rand,1,9,0,10);
    Reverse_String(hs_ciphertext->str);

    Rand64_Reset(rand);
    pseudoXOR(hs_ciphertext->str,rand);

    strncpy(output_buffer,hs_ciphertext->str,BUF_SIZE - 1);
    Free_HString(hs_ciphertext);

    return output_buffer;
}
