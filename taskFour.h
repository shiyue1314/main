#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <jerror.h>
#include <openssl/sha.h>


void bitStreamToBytes(unsigned char *, unsigned long , unsigned char *); 
    
void f5Decode(unsigned char *payload_bytes,unsigned long *permutation, unsigned long length,struct jpeg_decompress_struct cinfo_in,jvirt_barray_ptr *DCT_blocks) {

    unsigned char *bitStream;
    unsigned long bitLength = 0;
    unsigned long bsCounter;
    unsigned char bit = 0;
    unsigned long pmCounter;
    unsigned long position;
    int block_y, block_x;
    unsigned long block_index, index;
    short         value;
    bool          done = false;


    pmCounter = 0;
    //Get the bitStreamLength first
    for(bsCounter = 0; bsCounter < 32; bsCounter++){

        done = false;
        while(!done){
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

            //if the value is not zero || get the value and calculate the bitLength
            if(value){
                //fprintf(stderr, "[Debug] The value is %3d\n", value);
                bitLength += (value & 1) * (1 << bsCounter);
                done = true;
            }

            pmCounter++;
        }
    }

    fprintf(stderr, "[Debug] The bitLength is %ld.\n", bitLength);

    if((bitStream = malloc(bitLength)) == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
    }

    //After bitlength read 
    for(bsCounter = 0; bsCounter < bitLength; bsCounter++){

        done = false;
        while(!done){
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

            //if the value is not zero || get the value and calculate the bitLength
            if(value){
                //fprintf(stderr, "[Debug] The value is %3d\n", value);
                bitStream[bsCounter] = (value & 1); done = true;
            }

            pmCounter++;
        //end of while loop
        }

    //end of for loop
    }


    bitStreamToBytes(bitStream, bitLength, payload_bytes);

}


void bitStreamToBytes(unsigned char *bitStream, unsigned long bsLength, unsigned char *bytes) {
       
        unsigned int bytesLength;
        unsigned int byteCounter;
        unsigned int bitCounter;

        //Calculate the length of array of bytes
        bytesLength = bsLength / 8;

        for(byteCounter = 0; byteCounter < bytesLength; byteCounter++){
            //this is critical to give the byte default value to 0
            *bytes = 0;
            for(bitCounter = 0; bitCounter < 8; bitCounter++){
               // fprintf("Bits: %d\n", *bitStream);
                *bytes += (*bitStream++) * (1 << bitCounter);
               //fprintf(stderr, "Byte: %c\n", *bytes);
            }
            bytes++;
        }

        
        //restore the pointers
        bitStream -= (bsLength);
        bytes -= bytesLength;

        
        //fprintf(stderr, "[Content Debug] \n");
        //fprintf(stderr, "%s", bytes);

        //return bytes;

        for(byteCounter = 0; byteCounter < bytesLength; byteCounter++){
            //fprintf("Bytes: %d\n", bytes[byteCounter]);
        }
}

