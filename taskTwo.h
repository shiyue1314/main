#include <stdlib.h>
#include <string.h>
//#include <jpeglib.h>
#include <stdio.h>
//#include <jerror.h>
#include <openssl/sha.h>



unsigned long *gePermutation(unsigned char*, unsigned long);
unsigned long *permutationGenerator(unsigned long *, unsigned char *, unsigned long);
static unsigned long rand_int(unsigned long);

//Get permutation of length each element is type long
unsigned long *getPermutation(unsigned char *keyhash,unsigned long length) {
    
    unsigned long *permutation;

    if((permutation = malloc(length * sizeof(unsigned long))) == NULL){
        fprintf(stderr, "Error in allocate memory for permutation!");
    }

    //intilized permutation with increment order
    unsigned long i;

    for(i = 0; i < length; i++){
        permutation[i] = i;
    }

    permutationGenerator(permutation,keyhash, length);

    return permutation;
}


/* parameters:
 * permutation - takes array of long that need to permute
 * keyhas - hash need for random seed
 * length - the permutation length
 */
unsigned long *permutationGenerator(unsigned long *permutation, unsigned char *keyhash, unsigned long length) {
    unsigned long i, j, tmp;

    srand(*(unsigned int *) keyhash);

    //Knuth Shuffle random permutation||Durstenfeld's version of the algorithm
    for(i = length-1; i > 0; i --) {
        //Generate int
        j = rand_int(i+1);

        tmp = permutation[j];
        permutation[j] = permutation[i];
        permutation[i] = tmp;
    }

    return permutation;
}

//Generate random long between 0 to n, excluding n itself.
static unsigned long rand_int(unsigned long n) {
    unsigned long limit = RAND_MAX - RAND_MAX % n;
    unsigned long rnd;

    do {
        rnd = rand();
    } while (rnd >= limit);

    return rnd % n;
}



