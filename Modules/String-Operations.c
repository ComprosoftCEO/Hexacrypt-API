#include <String-Operations.h>
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
            ++temp_str;
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


char* hex_strdup(const char *c) {
    char *dup = calloc(strlen(c) + 1,sizeof(char));

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
    size_t len;

    if (Rand64_Next(garb_rand) & 7) {

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

        len = strlen(string->str);
        for (i = 0; i < back; ++i) {
            string->str[len + i] = allChars[Rand64_Next(garb_rand) % allChars_length];
        }

    } else {

        //Randomly put "corrupted" garbage as part of the algorithm
        //  This works because decryption does not remove garbage if it is
        //  outside the specified range
        front = front_min_garb + front_range_garb + 1;
        back  = back_min_garb + back_range_garb + 1;
        len = strlen(string->str) - back;
    }

    //Encode the garbage by appending onto back of key
    strcpy(key,allChars);
    Shuffle_String(rand,key,allChars_length);

    len += back;
    string->str[len] = key[front];
    string->str[len+1] = key[back];
    string->str[len+2] = 0;             //Always add the Null-Terminator
}



void Remove_Garbage(pHString string, pRand64 rand,
                unsigned int front_min_garb, unsigned int front_range_garb,
                unsigned int back_min_garb,  unsigned int back_range_garb) {

    static char key[sizeof(ALL_CHARS_LITERAL)];
    strcpy(key,allChars);

    Shuffle_String(rand,key,allChars_length);

    uint64_t front, back;
    size_t len = strlen(string->str);

    if (len < 2) {
        return;   /*This string has no garbage*/
    }

    front = strchr(key,string->str[len-2]) - key;
    back  = strchr(key,string->str[len-1]) - key;

    //Make sure the garbage is in range to confuse decryption
    //  It *should* still decrypt to complete garbage
    if (front < front_min_garb || back < back_min_garb ||
        front > (front_min_garb + front_range_garb) ||
        back > (back_min_garb + back_range_garb) ||
        (front+back) > len) {
            //TODO: Throw an error code??
            string->str[len - 2] = 0;       //Remove the two counters anyway
            return;
        }

    //Be sure to remove encoded garbage as well..
    string->str[(len - 2)- back] = 0;
    string->str+=front;
}



//Da buffer BETTER BE MODIFIABLE!!!
void Flip_Bytes(uint8_t* buf, size_t len) {
    uint8_t temp, *end_buf = buf+len-1;
    size_t i;

    /*Divide by 2, round down */
    len>>=1;
    for (i = 0; i < len; ++i) {
        temp = *buf;
        *buf = *end_buf;
        *end_buf = temp;

        ++buf;
        --end_buf;
    }
}

void Force_Endianess(uint8_t *buf, size_t length, Endian_t e) {

    //What is my endianess???
    Endian_t sys_e = E_BIG;
    BYTE_TYPE(uint32_t) tester = {.value = 1};
    if (tester.byte[0]) {sys_e = E_LITTLE;}

    //Flip Bytes if endians do not match
    if (e != sys_e) {Flip_Bytes(buf,length);}
}


#define STRING_TO_TEMPLATE(name,type) \
type String_To_##name(const char* str) { \
    BYTE_TYPE(type) buf; \
    \
    /* strncpy automatically fills rest in with 0's */ \
    strncpy((char*) buf.byte,str,sizeof(type));  \
    \
    Force_Endianess(buf.byte,sizeof(type),E_BIG); \
    \
    return buf.value;\
}

//len = Length of the arr
#define BYTES_TO_TEMPLATE(name,type) \
type Bytes_To_##name(const uint8_t* arr, size_t len) { \
    BYTE_TYPE(type) buf; \
    size_t i, copy_len = sizeof(type); \
    \
    if (len < copy_len) {copy_len = len;} \
    for (i = 0; i < copy_len; ++i) { /* Copy only size of array */ \
        buf.byte[i] = arr[i]; \
    } \
    \
    for (; i < sizeof(type); ++i) { /* Fill remaining with 0 */ \
        buf.byte[i] = 0; \
    } \
    \
    Force_Endianess(buf.byte,sizeof(type), E_BIG); \
    \
    return buf.value; \
}

STRING_TO_TEMPLATE(U16,uint16_t);
STRING_TO_TEMPLATE(U32,uint32_t);
STRING_TO_TEMPLATE(U64,uint64_t);

BYTES_TO_TEMPLATE(U16,uint16_t);
BYTES_TO_TEMPLATE(U32,uint32_t);
BYTES_TO_TEMPLATE(U64,uint64_t);
