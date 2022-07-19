//
// Created by Om Singh on 16/07/2022.
//

#include "steg_write.h"
#include <stdio.h>
#include <stdlib.h>

#define BITS_ENCODED_PER_SAMPLE 4
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

/* Test Data
unsigned int raw[12] = {0x56, 0x12, 0x23, 0x74, 0xFF, 0xAB, 0x56, 0x12, 0x23, 0x74, 0xFF, 0xAB};
unsigned char tohide[4] = {0x23, 0x23, 0x45, 0x67};
 */

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int writeBinary(size_t dataSize, size_t soundSize, char * sound, char * data)
{
    unsigned int dataMask;
    unsigned int soundMask;

    unsigned int soundIndex = 0;
    unsigned int dataIndex = 0;

    // Loops until we have written all data or run out of space
    while(dataIndex < dataSize && soundIndex < soundSize) {
        // Loops for each byte of data encoded
        for (int j = 0; j < 8 / BITS_ENCODED_PER_SAMPLE; j++) {
            // Loops for each bit of data encoded
            for (int bitIndex = 0; bitIndex < BITS_ENCODED_PER_SAMPLE; bitIndex++) {

                // dataMask, when AND'd with the data, will set all bits but one to 0
                dataMask = 1 << bitIndex;
                // soundMask, when AND'd with the sound, will set one bit to 0 (corresponds to the non-set bit in dataMask)
                soundMask = ~dataMask;

                printf("data[%d] = "BYTE_TO_BINARY_PATTERN"\n",dataIndex, BYTE_TO_BINARY(data[soundIndex]));
                printf("sound[%d] = "BYTE_TO_BINARY_PATTERN"\n",soundIndex, BYTE_TO_BINARY(sound[soundIndex]));

                printf("dataMask = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(dataMask));
                printf("soundMask = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(soundMask));

                // Replace the erased 0 in sound with the non-set bit in the data
                sound[soundIndex] = ((sound[soundIndex] & soundMask) | (data[dataIndex] & dataMask));

                printf("sound[%d] (edited) = "BYTE_TO_BINARY_PATTERN"\n",soundIndex, BYTE_TO_BINARY(sound[soundIndex]));
            }
            // Now that the correct amount of bits has been encoded, shift data along so that the next set of data
            // bits can be encoded
            data[dataIndex] = data[dataIndex] >> BITS_ENCODED_PER_SAMPLE;
            soundIndex++;
        }
        dataIndex++;
    }
    // Return the amount of data that is left to right
    return dataSize - dataIndex;
}


FILE *file;
unsigned char *buffer;

int writeSteg(filedata_t f, header_t h,const char * fileName)
{
    printf("Opening file... %s\n", fileName);
    file = fopen(fileName, "rb");

    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // Find the size (in bytes) of the file whose data
    // is going to be encrypted
    fseek( file , 0L , SEEK_END);
    int fileSize = ftell( file );
    rewind( file );

/* allocate memory for entire content */
    buffer = calloc( 1, fileSize+1 );
    if( !buffer ) fclose(file),fputs("memory alloc fails",stderr),exit(1);

/* copy the file into the buffer */
    if( 1!=fread( buffer , fileSize, 1 , file) )
        fclose(file),free(buffer),fputs("entire read fails",stderr),exit(1);

    setbuf(stdout, 0);
    printf("File size of %d bytes\n",fileSize);

    int soundSize = 0;
    for(int i = 0; i < f.noChannels; i++)
    {
        soundSize += f.channels[i].size * BITS_ENCODED_PER_SAMPLE;
    }

    if(fileSize > soundSize){
        printf("There isn't enough space in the sound file to encode the data file");
        return EXIT_FAILURE;
    }

    printf("Sound Size: %d\n",soundSize);

    int amountWritten = 0;

    // Iterate through the channels
    for(int i = 0; i < h.channels; i++)
    {
        amountWritten = writeBinary(fileSize-amountWritten,f.channels->size,f.channels[i].samples,buffer);

        if(amountWritten==0)
        {
            break;
        }
    }
    printf("Exiting writeSteg...");
    return EXIT_SUCCESS;
}

/*mask = (1 << param) - 1;*/