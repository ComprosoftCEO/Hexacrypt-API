# Hexacrypt-API
C shared library for the Hexacrypt algorithm

## Compiling and Running
This library is designed to work with the cross-platform IDE [Code::Blocks](http://www.codeblocks.org/). To compile, open Hexacrypt-API.cbp in Code::Blocks, and it should compile.

* On Windows, it compiles to __Hexacrypt-API.dll__
* On Mac/Linux, it compiles to __Hexacrypt-API.so__

When compiling for Windows, you might need to define the constant *\_WIN32*. In the Code::Blocks toolbar, go to Settings -> Compiler Settings -> Global Compiler Settings. Under the tab #defines, add the line *\_Win32*.

## Cross-Platform Support
This shared library is designed to work on any platform. The file Dll.h defines the various constants needed to compile the .dll on Windows or the .so file on Mac/Linux. The file Hexacrypt-API.c also contains the DllMain function needed by Windows DLL files.

## Debug vs Release
Hexacrypt-API defines 2 versions when compiling:

* **Debug** - Compile an executible to test the code (located in bin/Debug)
* **Release** - Compile the .dll or .so file (located in bin/Release)

The main function is defined in Hexacrypt-API.c, and helps to debug the code without having to use an external program to test the .dll or .so file. The file Hexacrypt-Tester.c is a useful command line utility that is provided for testing, but can be redefined to use a different main function.


## Algorithms Supported
* **Hexacrypt** - The origional [Hexacrypt](https://github.com/ComprosoftCEO/Hexacrypt/) algorithm as first programmed in Visual Basic. (*see other notes*)
* **Hexacrypt Plus** - A new algorithm that improves on the old, adding multiple rounds of encryption, message scrambling (called Matrix Code), key expansion, and a better garbage system

## Algorithms to Add
*  **Hexabyte** - Encrypt a binary string as opposed to a plain text string
*  **HexaHash** - Hash a string
*  **ByteHash** - Hash a binary string

## Other Notes
This version of Hexacrypt is __NOT COMPATIBLE__ with the Hexacrypt programmed for Visual Basic and PHP. In the process of writing Hexacrypt-API, I made some optimizations and other improvements to the origional Hexacrypt algorithm. My plan is to modify [Hexacrypt](https://github.com/ComprosoftCEO/Hexacrypt/) and [Hexacrypt-PHP](https://github.com/ComprosoftCEO/Hexacrypt-PHP) in the near future to reflect these changes.

Here is a list of what I changed:
* Removed the checksum
* PseudoXOR uses the previous scrambled key string to scramble the new key string
* The garbage can sometimes put "corrupted" garbage indexes on the string, and this decrypts just fine. Before, "corrupted" garbage returned an error.
* The encrypted message is guaranteed to decrypt, even if the key is incorrect. Often times, a bad key will decrypt to an empty string "".
