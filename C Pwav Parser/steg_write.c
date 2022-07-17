//
// Created by Om Singh on 16/07/2022.
//

#include "steg_write.h"

#define STEG_BITS_PER_SAMPLE 4;

FILE *file;
char buffer;
unsigned int mask = 0b11111111111111111111111100000000;

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

int writeSteg(filedata_t f, header_t h,const char * fileName)
{
    printf("Opening file... %s\n", fileName);
    file = fopen(fileName, "rb");

    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    printf("LOL");

    // Find the size (in bytes) of the file whose data
    // is going to be encrypted
    fseek(file, 0L, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    setbuf(stdout, 0);
    printf("File size of %d bytes\n",fileSize);

    int soundSize = 0;
    for(int i = 0; i < f.noChannels; i++)
    {
        soundSize += f.channels[i].size * STEG_BITS_PER_SAMPLE;
    }

    if(fileSize > soundSize){
        printf("There isn't enough space in the sound file to encode the data file");
        return EXIT_FAILURE;
    }

    printf("Sound Size: %d\n",soundSize);

    // Iterate through the channels
    for(int i = 0; i < h.channels; i++)
    {
        // Iterate through each sample in the channel
        for(int j = 0; j <= f.channels[i].size; j++)
        {
            if(fread(&buffer, 1, 1, file) == 1)
            {
                printf("Original String: ");
                printBits(sizeof(unsigned int),&f.channels[i].samples[j]);

                unsigned int temp = f.channels[i].samples[j] & mask;
                unsigned int output = temp | buffer;
                f.channels[i].samples[j] = output;

                printf("Modified String: ");
                printBits(sizeof(unsigned int),&f.channels[i].samples[j]);
            }
            else
            {
                break;
            }
        }
    }
    printf("Exiting writeSteg...");
    return EXIT_SUCCESS;
}

/*mask = (1 << param) - 1;*/