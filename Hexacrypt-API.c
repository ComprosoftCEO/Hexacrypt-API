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
    //#include "Hexacrypt-Tester.c"

    #include <stdio.h>
    #include <String-Operations.h>
    int main(int argc, char** argv) {

        if (argc < 2) {return 0;}

        uint32_t i = Bytes_To_U32(argv[1],3);

        printf("%d\n",i);


        //const char* enc = HexacryptPlus_Encrypt("Hello","Key",10);
        //printf("%s\n",enc);
        //const char* dec = HexacryptPlus_Decrypt(enc,"Key",10);
        //printf("%s\n",dec);

    }
#endif // DEBUG
