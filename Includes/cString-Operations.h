#ifndef C_STRING_OPERATIONS_HEADER
#define C_STRING_OPERATIONS_HEADER

#include <HString.h>
#include <cRand64.h>


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
char* strdup(const char* str);



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


#endif // C_STRING_OPERATIONS_HEADER Included
