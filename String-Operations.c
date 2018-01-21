#include <cString-Operations.h>
#include <string.h>


const char* allChars = " !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
const size_t allChars_length = sizeof(allChars) - 1;


void Filter_Ascii(char* string, size_t len) {
    char* temp_str = string;
    for (size_t i = 0; i < len; ++i) {

        //Filtered size is guaranteed <= current size, so just
        //  overwrite illegal characters and leave "Trash" past
        //  the Null-terminator
        if (string[i] >= 32 && string[i] <= 126) {
            *temp_str = string[i];
            temp_str++;
        }
    }
    *temp_str = 0;  //Null-terminator
}


//String needs to be pre-filtered or else it will fail
void pseudoXOR(char* string, pRand64 rand) {

    static char xor_string[sizeof(allChars)];
    size_t index;

    //Always recopy the string of all characters
    strcpy(xor_string,allChars);

    size_t len = strlen(string);
    for (size_t i = 0; i < len; ++i) {
        Shuffle_String(rand,xor_string,allChars_length);

        //TODO:   This should never fail, but maybe add check????
        index = strchr(xor_string,string[i]) - xor_string;
        string[i] = xor_string[(allChars_length - 1) - index];
    }
}




//  Length is passed in as a parameter b/c the main string being shuffled
//   is the allChars string, which always has the same length, so why bother
//   to always need to recalculate it. Yeah, it breaks encapsulation. I know...
//
// This means that the string does NOT need to be null-terminated
void Shuffle_String(pRand64 rand, char* string, size_t len) {
	char temp;

	for (size_t i = 0; i < len; ++i) {

		//Do some fancy pointer maths to make this swapping very quick
		uint64_t index = Rand64_NextInt(rand) % (len - i);

		temp = *string;
		*string = string[index];
		string[index] = temp;

		++string;
	}
}



void Reverse_String(char* string) {
    size_t len = strlen(string);
    for (size_t i = 0; i < len/2; ++i) {
        char temp = string[i];
        string[i] = string[(len-1)-i];
        string[(len-1)-i] = temp;
    }
}




//********************************************************
//This assumes that margins are pre-allocated
//********************************************************

void Add_Garbage(char* string,pRand64 rand) {


}
