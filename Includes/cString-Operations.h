#ifndef C_STRING_OPERATIONS_HEADER
#define C_STRING_OPERATIONS_HEADER

#include <HString.h>
#include <cRand64.h>


void Filter_Ascii(char* string, size_t len);
void pseudoXOR(char* string, pRand64 rand);
void Shuffle_String(pRand64 rand, char* string, size_t len);
void Reverse_String(char* string);

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
#define DEFAULT_FGARB_RANGE 10
#define DEFAULT_BGARB_MIN 0
#define DEFAULT_BGARB_RANGE 10


#endif // C_STRING_OPERATIONS_HEADER Included
