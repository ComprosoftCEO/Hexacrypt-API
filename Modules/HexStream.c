#include <HexStream.h>

#include <String-Operations.h>
#include <Rand64.h>
#include <Hash8.h>
#include <string.h>

//Private HexStream object type
typedef struct {
    char* state;        // Stores current location in the state buffer
    char* state_buf;    // Pointer to the start of the state buffer
    char* init_state;   // Used to easily reset back to the default values
    uint32_t size;      // Total size of the state
    uint32_t index;     // Where is state relative to state_buf
    pRand64 char_rand;  // Generates the next character in the sequence
} HexStream_Obj, *pHexStream_Obj;

extern const char* allChars;
extern const size_t allChars_length;


pHexStream New_HexStream(const char* seed, uint32_t state) {

    pHexStream_Obj hs = malloc(sizeof(HexStream_Obj));
    if (!state) {state = DEFAULT_STATE;}

    hs->size = state;

    // Buffer has pace for 2x the buffer
    //   Allocate the initial state with one calloc as well...
    hs->state_buf = malloc(state*3 * sizeof(char));
    hs->init_state = hs->state_buf + (state*2);
    hs->char_rand = New_Rand64_Seed(0);

    HexStream_Reseed((pHexStream) hs, seed);

    return (pHexStream) hs;
}



void Free_HexStream(pHexStream stream) {
    pHexStream_Obj hs = (pHexStream_Obj) stream;

    free(hs->state_buf);
    Free_Rand64(hs->char_rand);
    free(hs);
}


void HexStream_Reset(pHexStream stream) {
    pHexStream_Obj hs = (pHexStream_Obj) stream;

    uint32_t i;
    for (i = 0; i < hs->size; ++i) {
        hs->state_buf[i] = hs->init_state[i];
    }

    hs->state = hs->state_buf;
    hs->index = 0;

    Rand64_Reset(hs->char_rand);
}


void HexStream_Reseed(pHexStream stream, const char* seed) {

    pHexStream_Obj hs = (pHexStream_Obj) stream;

    //Reset the buffers and PRNG
    hs->state = hs->state_buf;
    hs->index = 0;
    Rand64_Reseed(hs->char_rand,Hash8_U64(seed));

    //Fill the state
    size_t i, len = strlen(seed);
    for (i = 0; (i < hs->size) && (i < len); ++i) {
        hs->state_buf[i] = seed[i];
        hs->init_state[i] = seed[i];
    }

    //Fill in remaining spaces with random characters
    for (; (i < hs->size); ++i) {
        hs->state_buf[i] = allChars[Rand64_Next(hs->char_rand) % allChars_length];
        hs->init_state[i] = hs->state_buf[i];
    }

    //Do one call to avoid finagling
    HexStream_Next((pHexStream) hs);
}


char HexStream_Next(pHexStream stream) {

    char ret;
    static pRand64 pxor_rand = NULL;
    pHexStream_Obj hs = (pHexStream_Obj) stream;

    if (!pxor_rand) {pxor_rand = New_Rand64();}

    //pseudoXOR the state using the state as the key
    //  Then get the first character
    Rand64_Reseed(pxor_rand,Hash8_U64_Length(hs->state,hs->size));
    pseudoXOR_Length(hs->state,hs->size,pxor_rand);
    ret = hs->state[0];

    //Shift every character down by 1 and add a "Random" char
    //  onto the end

    // To speed up this process, simply move the pointer
    if (++hs->index > hs->size) {
        memcpy(hs->state_buf,hs->state+1,hs->size);
        hs->state = hs->state_buf;
        hs->index = 0;
    } else {
        ++hs->state;
    }

    char random_char = allChars[Rand64_Next(hs->char_rand) % allChars_length];
    hs->state[hs->size - 1] = random_char;

    return ret;
}


void HexStream_NextString(pHexStream stream, char* buf, size_t count) {
    //Add null terminator
    for (size_t i = 0; i < count; ++i) {
        buf[i] = HexStream_Next(stream);
    }
}


uint32_t HexStream_GetInt(pHexStream stream) {

    uint32_t retVal = 0;

    for (size_t i = 0; i < sizeof(uint32_t) * 2; ++i) {
        char c;
        while ((c = (HexStream_Next(stream) - 31)) > 16*5);
        retVal <<= 4;
        retVal |= (c & 0xF);
    }

    return retVal;
}


uint64_t HexStream_GetLong(pHexStream stream) {

    uint64_t retVal = 0;

    for (size_t i = 0; i < sizeof(uint64_t) * 2; ++i) {
        char c;
        while ((c = (HexStream_Next(stream) - 31)) > 16*5);
        retVal <<= 4;
        retVal |= (c & 0xF);
    }

    return retVal;
}
