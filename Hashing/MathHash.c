#include <MathHash.h>
#include <stdio.h>
#include <stdlib.h>


#define DEFAULT_COUNT 10
#define DEFAULT_SIDE   3

#define CUBE(hash,index,x,y,z) \
    hash->arr[index][(hash->sz_plane * z) + (hash->sz_side * y) + x]


typedef struct {
    uint16_t count;          /* Total number of chunks (default is 10)*/
    uint16_t sz_side;        /* Side length for each chunk cube (default is 3)*/
    uint32_t sz_plane;       /* How big is a plane (side*side) */
    uint32_t sz_cube;        /* How big is a cube (plane*side) */
    uint32_t **arr;          /* Array of cubes */

    //Use the following for absorption and squeezing
    uint16_t current;        /* Current cube to absorb into */
} MathHash_Obj, *pMathHash_Obj;


pMathHash New_MathHash(uint8_t side, uint16_t count) {

    pMathHash_Obj hash = calloc(1,sizeof(MathHash_Obj));

    if (count) {hash->count = count;} else {hash->count   = DEFAULT_COUNT;}
    if (side) {hash->sz_side = side;} else {hash->sz_side = DEFAULT_SIDE;}

    hash->sz_plane = ((uint32_t) hash->sz_side) * ((uint32_t) hash->sz_side);
    hash->sz_cube = hash->sz_plane * ((uint32_t) hash->sz_side);

    //Allocate the array of integers
    hash->arr = (uint32_t**) calloc(hash->count,sizeof(uint32_t*));
    for (uint32_t i = 0; i < hash->count; ++i) {
        hash->arr[i] = (uint32_t*) calloc(hash->sz_cube,sizeof(uint32_t));
    }

    return (pMathHash) hash;
}


void Free_MathHash(pMathHash mh) {
    if (!mh) {return;}
    pMathHash_Obj hash = (pMathHash_Obj) mh;

    uint32_t i;
    for (i = 0; i < hash->count; ++i) {
        free(hash->arr[i]);
    }

    free(hash->arr);
    free(hash);
}


void MathHash_Print(pMathHash mh) {
    if (!mh) {return;}
    pMathHash_Obj hash = (pMathHash_Obj) mh;

    uint16_t i;
    uint32_t cube_index;
    for (i = 0; i < hash->count; ++i) {
        printf("======================\n"
               ":     Cube %-5hu     :\n"
               "======================\n",i);

        for (cube_index = 0; cube_index < hash->sz_cube; ++cube_index) {
            if ((cube_index % hash->sz_plane) == 0) {printf("|");}
            printf("%u",hash->arr[i][cube_index]);

            if ((cube_index % hash->sz_plane) == (hash->sz_plane - 1)) {
                printf("|\n\n");
            } else {
                if ((cube_index % hash->sz_side) == (hash->sz_side - 1)) {
                    printf("|\n|");
                } else {
                    printf(",");
                }
            }
        }
    }


}


void MathHash_Absorb(pMathHash mh, uint32_t data) {
    if (!mh) {return;}
    pMathHash_Obj hash = (pMathHash_Obj) mh;

    //The new data goes into the middle of the chunk
    uint16_t middle = (hash->sz_side) >> 1;
    CUBE(hash,hash->current,middle,middle,middle) = data;



}
