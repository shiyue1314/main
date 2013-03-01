#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <jerror.h>
#include <openssl/sha.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0


// constants for the embed/extract mode
void f5Embed(unsigned char *bitStream, unsigned long bitStreamLength, unsigned long *permutation, unsigned long permutationLength, struct jpeg_decompress_struct cinfo_in, jvirt_barray_ptr *DCT_blocks) {

    unsigned long  bSCounter;
    unsigned long  pmCounter = 0;
    unsigned char  bit;
    short          value;
    unsigned long  position;
    int block_y, block_x;
    unsigned long block_index, index;

    bool done;

    fprintf(stderr, "[Debug] size of bitStream: %ld\n", bitStreamLength);
    fprintf(stderr, "[Debug] size of permutation: %ld\n", permutationLength);

    for(bSCounter = 0; bSCounter < bitStreamLength; bSCounter++){
        bit = bitStream[bSCounter];

        //flag for singal embeded 
        done = false;

        //fprintf(stderr, "the bit is %d\n", bit);
        //under the assumption that permutation is enough to encode the bitStream | need improvement
        while(!done){

            block_y = 0;
            block_x = 0;
            block_index = 0;
            index = 0;

            //Get current permutation position
            position = permutation[pmCounter];
            //fprintf(stderr, "Position is %ld\n", position);

            //Get block_y and block_x;
            block_index= position / 64;

            //The width is 100
            //Get the row value
            block_y = block_index / 100;
            //Get the col value
            block_x = block_index % 100;

            //Get index [u, v]
            index = position % 64;

            //debug done
            //fprintf(stderr, "The index is %ld\n", index);
            //fprintf(stderr, "The block_y is %ld\n", block_y);
            //fprintf(stderr, "The block_x is %ld\n", block_x);

            //Use the magic function to get the block
            JCOEFPTR block=(cinfo_in.mem->access_virt_barray)((j_common_ptr)&cinfo_in, DCT_blocks[0], block_y, (JDIMENSION)1, FALSE)[0][block_x];

            //Get the value of this position 
            value = block[index];

            //fprintf(stderr, "The VALUE is: %3d \n", value );

            //if the value is not 0 (false) --> skip the zero coefficience
            if(value){
                //F5 embedding

                //debug
                //fprintf(stderr, "[Embeding] Value was %3d | ", value);

                //if the bit not equal to the LSB of value
                //!becareful tha -1%2 == -1 not 1
                if( bit != abs((value % 2))){

                    //if value is bigger than 0, value = value - 1;
                    if (value > 0){
                        value--;
                    } else {
                        //else value = value + 1
                        value++;
                    }
                }

                //write the value to the block
                block[index] = value;

                //debug
                //fprintf(stderr, "is %3d | ", value);
                //fprintf(stderr, "block value is %3d | ", block[index]);
                //fprintf(stderr, "Bit is %d\n", bit);
                
                //if the after change is not 0 --> done = true
                if(value){
                    //fprintf(stderr, "--------\n");
                    done = true;
                }
            }
            //end of while loop    

            //get next permutation
            pmCounter++;
        }

        /*
        block_y = 21;
        block_x = 07;

        JCOEFPTR block=(cinfo_in.mem->access_virt_barray)((j_common_ptr)&cinfo_in, DCT_blocks[0], block_y, (JDIMENSION)1, FALSE)[0][block_x];

        fprintf(stderr, "[UUUU] %3d\n", block[10]);
        */
    //end of for loop
    }

}
