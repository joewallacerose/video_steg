//
// Created by Om Singh on 15/07/2022.
//

#ifndef C_PWAV_PARSER_WAVE_H
#define C_PWAV_PARSER_WAVE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// WAVE file header format
typedef struct HEADER {
    unsigned char riff[4];						// RIFF string
    unsigned int overall_size	;				// overall size of file in bytes
    unsigned char wave[4];						// WAVE string
    unsigned char fmt_chunk_marker[4];			// fmt string with trailing null char
    unsigned int length_of_fmt;					// length of the format data
    unsigned int format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
    unsigned int channels;						// no.of channels
    unsigned int sample_rate;					// sampling rate (blocks per second)
    unsigned int byterate;						// SampleRate * NumChannels * BitsPerSample/8
    unsigned int block_align;					// NumChannels * BitsPerSample/8
    unsigned int bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header [4];		// DATA string or FLLR string
    unsigned int data_size;// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} header_t;

typedef struct CHANNEL {
    unsigned int size;
    unsigned int *samples;
}channel_t;

typedef struct FILEDATA {
    unsigned int noChannels;
    channel_t *channels;
}filedata_t;

header_t getHeader(FILE *fp);

filedata_t getFileData(FILE *fp, header_t header);

void writeFile(header_t header, filedata_t data);

#endif //C_PWAV_PARSER_WAVE_H
