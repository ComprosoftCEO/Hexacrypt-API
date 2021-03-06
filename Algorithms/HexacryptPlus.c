//===================================================
// Super advanced Hexacrypt encryption algorithm
//===================================================
//
//  [Plaintext]----------->V
//                  /->Pseudo XOR -> Add Garbage
//  [Key]----------/-->Matrix Code <-----/
//    V (expand)           V
//  [Key']----------\->Pseudo XOR -> Add Garbage
//    V (expand)     \->Matrix Code <----/
//
//            (and so on...)
#include <Hexacrypt-API.h>

#include <Hash8.h>
#include <Rand64.h>
#include <String-Operations.h>
#include <MatrixCode.h>
#include <KeyExpand.h>

#include <stdlib.h>
#include <string.h>


DLL_EXPORT const char* HexacryptPlus_Encrypt(const char* plaintext, const char* key,
                            uint32_t rounds) {

    pHString hs_plaintext = New_HString(plaintext,BUF_SIZE,rounds,4,4);
    pKeyExpand ke = New_KeyExpander(key);
    static pRand64 rand = NULL;

    if (!rand) {rand = New_Rand64_Seed(0);}

    for (uint32_t i = 0; i < rounds; ++i) {

        Rand64_Reseed(rand,Hash8_U64(NULL,ke->str));
        pseudoXOR(hs_plaintext->str,rand);

        Rand64_Reset(rand);
        Reverse_String(hs_plaintext->str);
        Add_Garbage(hs_plaintext,rand,
                    DEFAULT_FGARB_MIN, DEFAULT_FGARB_RANGE,
                    DEFAULT_BGARB_MIN, DEFAULT_BGARB_RANGE);

        MatrixCode(hs_plaintext->str,ke->str,DEFAULT_COMMANDS);

        ExpandKey(ke);
    }

    //One final PseudoXOR
    Rand64_Reseed(rand,Hash8_U64(NULL,ke->str));
    pseudoXOR(hs_plaintext->str,rand);


    //Avoid memory leaks
    strncpy(output_buffer,hs_plaintext->str,BUF_SIZE - 1);
    Free_HString(hs_plaintext);
    Free_KeyExpander(ke);

    return output_buffer;
}



DLL_EXPORT const char* HexacryptPlus_Decrypt(const char* ciphertext, const char* key, uint32_t rounds) {

    //Go back and recalculate all former keys
    pKeyExpand ke = New_KeyExpander(key);
    char** all_keys = malloc(sizeof(char*) * (rounds+1));
    uint32_t i;
    for (i = 0; i <= rounds; ++i) {
        all_keys[i] = hex_strdup(ke->str);
        ExpandKey(ke);
    }

    static pRand64 rand = NULL;
    if (!rand) {rand = New_Rand64_Seed(0);}

    //Add No extra garbage space to the string
    pHString hs_ciphertext = New_HString(ciphertext,BUF_SIZE,0,0,0);

    Rand64_Reseed(rand,Hash8_U64(NULL,all_keys[rounds]));
    pseudoXOR(hs_ciphertext->str,rand);

    for (i = rounds; i > 0; --i) {

        MatrixCode_Inverse(hs_ciphertext->str,all_keys[i-1],DEFAULT_COMMANDS);

        Rand64_Reseed(rand,Hash8_U64(NULL,all_keys[i-1]));
        Remove_Garbage(hs_ciphertext,rand,
                    DEFAULT_FGARB_MIN, DEFAULT_FGARB_RANGE,
                    DEFAULT_BGARB_MIN, DEFAULT_BGARB_RANGE);

        Reverse_String(hs_ciphertext->str);


        Rand64_Reset(rand);
        pseudoXOR(hs_ciphertext->str,rand);

    }

    //Avoid memory leaks
    strncpy(output_buffer,hs_ciphertext->str,BUF_SIZE - 1);
    Free_HString(hs_ciphertext);
    Free_KeyExpander(ke);

    for (i = 0; i <= rounds; ++i) {
        free(all_keys[i]);
    } free(all_keys);

    return output_buffer;
}
