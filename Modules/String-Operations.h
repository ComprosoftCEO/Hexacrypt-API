#ifndef STRING_OPERATIONS_HEADER
#define STRING_OPERATIONS_HEADER

#include <HString.h>
#include <Rand64.h>


/** \brief Filter a string to only have Ascii characters (32 - 126)
 *
 * \param string The string to filter
 * \param len The length of the string
 */
void Filter_Ascii(char* string, size_t len);


void pseudoXOR(char* string, pRand64 rand);
void pseudoXOR_Length(char* string, size_t len, pRand64 rand);


/** \brief Do the PseudoXOR operation on every Nth Character in the string
 *
 * \param string The String to modify
 * \param incrament Modify every Nth character
 * \param count How many total characters to incrament
 * \param rand Random object to shuffle key space
 */
void pseudoXOR_NChar(char* string, size_t incrament, size_t count, pRand64 rand);

/** \brief Shuffle a string using the given Rand64 object
 *
 * \param rand Random Number Generator to use for shuffling
 * \param string The string to shuffle
 * \param len Length of the string to shuffle
 */
void Shuffle_String(pRand64 rand, char* string, size_t len);


/** \brief Reverse the order of the characters in the string
 *
 * \param string The string to reverse
 */
void Reverse_String(char* string);


/** \brief Duplicate a string and return a new allocate buffer
 *
 * \param str The string to duplicate
 * \return Newly allocated string, needs to be freed.
 */
char* hex_strdup(const char* str);



/** \brief Add random characters to the front and back of the string
 *
 * The garbage added on front or back MUST BE less than 95. Otherwise, it
 *   cannot be encoded
 *
 * \param string The Hexacrypt string to modify
 * \param rand Random Number Generator used to encode the key
 *
 * \param front_min_garb Minimum amount of garbage to add to the front of string
 * \param front_range_garb Add min_garb to (min_garb + range_garb) to the front of the string, inclusive
 * \param back_min_garb Minimum amount of garbage to add to the back of string
 * \param back_range_garb Add min_garb to (min_garb + range_garb) to the back of the string, inclusive
 */
void Add_Garbage(pHString string, pRand64 rand,
                 unsigned int front_min_garb, unsigned int front_range_garb,
                 unsigned int back_min_garb,  unsigned int back_range_garb);




void Remove_Garbage(pHString string, pRand64 rand,
                unsigned int front_min_garb, unsigned int front_range_garb,
                unsigned int back_min_garb,  unsigned int back_range_garb);


#define DEFAULT_FGARB_MIN 1
#define DEFAULT_FGARB_RANGE 3
#define DEFAULT_BGARB_MIN 0
#define DEFAULT_BGARB_RANGE 4




/** \brief Reverse <length> bytes inside a buffer
 *
 * \param buf The buffer to modify when reversing byte order
 * \param len The number of bytes to reverse
 */
void Flip_Bytes(uint8_t* buf, size_t len);



typedef enum {
    E_LITTLE,
    E_BIG
} Endian_t;

/** \brief Force N number of bytes in buf to be in a certain endian order.
 *
 *  If e == system endianess, then this function does nothing. Otherwise, it
 *    reverses <length> bits from buf (thus modifying buf)
 *
 * \param buf The buffer of uint8 values to modify
 * \param length Number of bits to modify
 * \param e Target endianess
 */
void Force_Endianess(uint8_t *buf, size_t length, Endian_t e);

//Special union that has both a type and an array
//  of bytes for that type
#define BYTE_TYPE(type) \
union { \
    type value; \
    uint8_t byte[sizeof(type)];\
}


/** \brief Convert a string or byte array into a different data type
 *
 *   With the String_To_xxx and Bytes_To_xxx functions, the bytes of a char* is converted into
 *    the respective xxx data type. If a string does not have as many bytes
 *    as the output data type, the remaining bytes are filled with 0x00.
 *
 *   The data type is considered to be in big endian order, but is converted
 *    based on the target platform
 *
 *   ex) "Hello" -> {48 65 6c 6c 6f}
 *        U16 -> {48 65}                   = 18533
 *        U32 -> {48 65 6c 6c}             = 1214606444
 *        U64 -> {48 65 6c 6c 6f 00 00 00} = 5216694956353126400
 *
 * \param str The string to convert
 * \param len Total size of the byte array (BYTES_TO_xxx only)
 * \return Respective data type
 */
#define STRING_TO_FUNCTION(name,type) type String_To_##name(const char* str)
#define BYTES_TO_FUNCTION(name,type) type Bytes_To_##name(const uint8_t* arr, size_t len)

STRING_TO_FUNCTION(U16,uint16_t);
STRING_TO_FUNCTION(U32,uint32_t);
STRING_TO_FUNCTION(U64,uint64_t);

BYTES_TO_FUNCTION(U16,uint16_t);
BYTES_TO_FUNCTION(U32,uint32_t);
BYTES_TO_FUNCTION(U64,uint64_t);


#endif // STRING_OPERATIONS_HEADER Included
