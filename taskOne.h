#include <stdio.h>
#include <stdlib.h>


unsigned char *bytesToBitStream(char *, unsigned int );
//unsigned char *bitStreamToBytes(char *);

//convert a byte array with bytesLength into bit stream.
unsigned char *bytesToBitStream(char *bytes, unsigned int bytesLength) {
       unsigned char *bitStream;
       unsigned int bitsLength;
       unsigned int byteCounter;
       unsigned int bitCounter;

       bitsLength = bytesLength * 8;

       //allocate the memory for bitstream
       if((bitStream = malloc(bitsLength+32)) == NULL){
            fprintf(stderr, "Memory allocation failed!\n");
       }

       //encode the length informtion in the first 32 byte 
       for(bitCounter = 0; bitCounter < 32; bitCounter++) {
            *bitStream++ = (bitsLength & (1 << bitCounter)) != 0;
       }

       //convert the array of bytes into bitStream
       for(byteCounter = 0; byteCounter < bytesLength; byteCounter++){
          for(bitCounter = 0; bitCounter < 8; bitCounter++ ){
            *bitStream++ = (*bytes & (1 << bitCounter)) != 0;
          }
          bytes += 1;
       }


       //restore the pointers to original positions
       bytes -= bytesLength;
       bitStream -= (bitsLength+32);

       //return the bitstream point
       return bitStream;
 }

//convert the bitstream to original array of bytes
unsigned char *abitStreamToBytes(char *bitStream) {
       
        char *bytes;
        unsigned int bytesLength;
        unsigned int bitsLength;
        unsigned int byteCounter;
        unsigned int bitCounter;

        //read out the bit stream lengths
        bitsLength = 0;
        for(bitCounter = 0; bitCounter < 32; bitCounter++){
           bitsLength += (*bitStream++) * (1 << bitCounter);
        }

        fprintf(stderr, "[Debug in taskOne.h]BitLengths: %d\n", bitsLength);

        //Calculate the length of array of bytes
        bytesLength = bitsLength / 8;

        if((bytes = malloc(bytesLength)) == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
        }

        for(byteCounter = 0; byteCounter < bytesLength; byteCounter++){
            //this is critical to give the byte default value to 0
            *bytes = 0;
            for(bitCounter = 0; bitCounter < 8; bitCounter++){
               // fprintf("Bits: %d\n", *bitStream);
                *bytes += (*bitStream++) * (1 << bitCounter);
               //fprintf(stderr, "Byte: %c\n", *bytes);
            }
            bytes += 1;
        }

        
        //restore the pointers
        bytes -= bytesLength;
        bitStream -= (bitsLength+32);
        
        fprintf(stderr, "[Content Debug] \n");
        fprintf(stderr, "%s", bytes);

        return bytes;

        for(byteCounter = 0; byteCounter < bytesLength; byteCounter++){
            //fprintf("Bytes: %d\n", bytes[byteCounter]);
        }
}

