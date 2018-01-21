#ifndef HEXACRYPT_DLL_HEADER
#define HEXACRYPT_DLL_HEADER

// Used for Windows, when to build a .dll file
//  Does nothing on Linux

#if defined(_WIN32) || defined(_WIN64)

    #include <windows.h>

    //Debug should not mangle the file
    #ifdef DEBUG
        #define DLL_EXPORT
        #define API_EXPORT
    #else
        #ifdef BUILD_DLL
            #define DLL_EXPORT __declspec(dllexport) __cdecl
            #define API_EXPORT __declspec(dllexport)
        #else
            #define DLL_EXPORT __declspec(dllimport) __cdecl
            #define API_EXPORT __declspec(dllimport)
        #endif
    #endif
#else
    //This is a Linux OS, so do nothing
    #define DLL_EXPORT
#endif

#endif // HEXACRYPT_DLL_HEADER Included
