//
// Created by Om Singh on 16/07/2022.
//

#include "steg_write.h"
#define BITS_ENCODED_PER_NUMBER 4
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


int readBinary(int bitsPerItem, int numBytesToEncode, unsigned int * soundData ,unsigned char * hiddenData) {

    unsigned int numEncoded = 0;
    unsigned int soundCounter = 0;

    unsigned int mask;

    for (int i = 0; i < numBytesToEncode; i++){
        for (int j = 0; j < bitsPerItem; j++){

            printf("RAW[counter] = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(soundData[soundCounter]));
            printf("hiddenData[j] = %d "BYTE_TO_BINARY_PATTERN"\n", j,  BYTE_TO_BINARY(hiddenData[i]));

            mask = ~(1 << numEncoded);
            soundData[soundCounter] = (soundData[soundCounter] & mask) | ( (hiddenData[i] >> j) & 0x1 );
            numEncoded += 1;

            printBits(sizeof(soundData[soundCounter]),&soundData[soundCounter]);

            printf("AFTER sound[counter] = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(soundData[soundCounter]));

            printf("numEncoded soundCounter : %u %u\n", numEncoded, soundCounter);

            if (numEncoded >= BITS_ENCODED_PER_NUMBER) {
                numEncoded = 0;
                soundCounter += 1;
            }

        }
    }

    for (int i = 0; i < 12 ; i ++){
        printf("ENCODED[%d] : %x\n", i, soundData[i]);
    }
    return EXIT_SUCCESS;
}

int readSteg(filedata_t f, header_t h,const char * fileName)
{
    return 0;
}

/*mask = (1 << param) - 1;*/