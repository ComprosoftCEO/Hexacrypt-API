#include <cString-Operations.h>

#include <string.h>


#define ALL_CHARS_LITERAL " !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

const char* allChars = ALL_CHARS_LITERAL;
const size_t allChars_length = sizeof(ALL_CHARS_LITERAL) - 1;


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

    static char xor_string[sizeof(ALL_CHARS_LITERAL)];
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


void pseudoXOR_Length(char* string, size_t length, pRand64 rand) {

    static char xor_string[sizeof(ALL_CHARS_LITERAL)];
    size_t index;

    //Always recopy the string of all characters
    strcpy(xor_string,allChars);

    for (size_t i = 0; i < length; ++i) {
        Shuffle_String(rand,xor_string,allChars_length);

        //TODO:   This should never fail, but maybe add check????
        index = strchr(xor_string,string[i]) - xor_string;
        string[i] = xor_string[(allChars_length - 1) - index];
    }
}

void pseudoXOR_NChar(char* string, size_t incrament, size_t count, pRand64 rand) {
    static char xor_string[sizeof(ALL_CHARS_LITERAL)];
    size_t index;

    //Always recopy the string of all characters
    strcpy(xor_string,allChars);

    count*=incrament;
    for (size_t i = 0; i < count; i+=incrament) {
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
		uint64_t index = Rand64_Next(rand) % (len - i);

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


char *strdup(const char *c) {
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL) {
       strcpy(dup, c);
    }

    return dup;
}



//********************************************************
//This assumes that margins are pre-allocated
//********************************************************
void Add_Garbage(pHString string, pRand64 rand,
                 unsigned int front_min_garb, unsigned int front_range_garb,
                 unsigned int back_min_garb,  unsigned int back_range_garb) {

    //Use a static random object for various things
    static char key[sizeof(ALL_CHARS_LITERAL)];
    static pRand64 garb_rand = NULL;
    if (!garb_rand) {garb_rand = New_Rand64();}

    uint64_t front, back, i;

    //Randomly pick the garbage
    front = front_min_garb +
            ((front_range_garb > 0) ? Rand64_Next(garb_rand) % front_range_garb : 0);

    back =  back_min_garb +
            ((back_range_garb > 0) ? Rand64_Next(garb_rand) % back_range_garb : 0);


    //HERE!!! It assumes there is enough space in buffer
    //  Otherwise, all sorts of BAD may happen
    string->str -=front;
    for (i = 0; i < front; ++i) {
        string->str[i] = allChars[Rand64_Next(garb_rand) % allChars_length];
    }

    size_t len = strlen(string->str);
    for (i = 0; i < back; ++i) {
        string->str[len + i] = allChars[Rand64_Next(garb_rand) % allChars_length];
    }


    //Encode the garbage by appending onto back of key
    strcpy(key,allChars);
    Shuffle_String(rand,key,allChars_length);

    len += back;
    string->str[len] = key[front];
    string->str[len+1] = key[back];
}



void Remove_Garbage(pHString string, pRand64 rand,
                unsigned int front_min_garb, unsigned int front_range_garb,
                unsigned int back_min_garb,  unsigned int back_range_garb) {

    static char key[sizeof(ALL_CHARS_LITERAL)];
    strcpy(key,allChars);

    Shuffle_String(rand,key,allChars_length);

    uint64_t front, back;
    size_t len = strlen(string->str);

    front = strchr(key,string->str[len-2]) - key;
    back  = strchr(key,string->str[len-1]) - key;

    //Make sure the garbage is in range to confuse decryption
    //  It *should* still decrypt to complete garbage
    if (front < front_min_garb || back < back_min_garb ||
        front > (front_min_garb + front_range_garb) ||
        back > (back_min_garb + back_range_garb) ||
        (front+back) > len) {

            //TODO: Throw an error code??
            return;
        }

    //Be sure to remove encoded garbage as well..
    string->str[(len - 2)- back] = 0;
    string->str+=front;
}


