#include <stdio.h>
#include <stdlib.h>

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


unsigned int raw[12] = {0x56, 0x12, 0x23, 0x74, 0xFF, 0xAB, 0x56, 0x12, 0x23, 0x74, 0xFF, 0xAB};
uint8_t tohide[4] = {0x01, 0x23, 0x45, 0x67};


int main() {
    int numBytesToEncode = 4;
    int bitsPerItem = 8;

    unsigned int numEncoded = 0;
    unsigned int rawCounter = 0;

    unsigned int mask;

    for (int i = 0; i < numBytesToEncode; i ++){
        for (int j = 0; j < bitsPerItem; j++){
            printf("RAW[counter] = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(raw[rawCounter]));
            printf("toHide[j] = %d "BYTE_TO_BINARY_PATTERN"\n", j,  BYTE_TO_BINARY(tohide[i]));

            mask = ~(1 << numEncoded);
            raw[rawCounter] = (raw[rawCounter] & mask) | ( (tohide[i] >> j) & 0x1 );
            numEncoded += 1;

            printf("AFTER RAW[counter] = "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(raw[rawCounter]));

            printf("numEncoded rawCounter : %u %u\n", numEncoded, rawCounter);

            if (numEncoded >= BITS_ENCODED_PER_NUMBER) {
                numEncoded = 0;
                rawCounter += 1;
            }

        }
    }

    for (int i = 0; i < 12 ; i ++){
        printf("ENCODED[%d] : %x\n", i, raw[i]);
    }

}
