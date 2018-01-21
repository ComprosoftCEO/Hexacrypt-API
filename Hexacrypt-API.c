// Handles any processes necessary to make this a DLL
//   or a .so file
#include <Hexacrypt-API.h>


//Special DLL initialization code
#if defined(_WIN32) || defined(_WIN64)
    API_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason){
        case DLL_PROCESS_ATTACH:    // Attach to process (return False to fail DLL load)
        case DLL_PROCESS_DETACH:    // detach from process
        case DLL_THREAD_ATTACH:     // attach to thread
        case DLL_THREAD_DETACH:     // detach from thread
            break;
    }
    return TRUE; // succesful
}
#endif

//Buffer used by all encryption and decryption
char output_buffer[BUF_SIZE];


//Compile a main if this is a debug program
#ifdef DEBUG

#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {

    if (argc < 3) {return 1;}

    for (int i = 0; i < 32; ++i) {
        char* encoded = Hexacrypt_Encrypt(argv[1], argv[2]);
        printf("\n********%s\n",encoded); fflush(stdout);

        char* decoded = Hexacrypt_Decrypt(encoded,argv[2]);
        printf("\n********%s\n",decoded); fflush(stdout);

        //free(encoded);
        //free(decoded);
    }
}


#endif // DEBUG
