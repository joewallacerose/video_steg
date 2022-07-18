/**
 * Read and parse a wave file
 *
 **/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"
#define TRUE 1
#define FALSE 0

// WAVE header structure
unsigned char buffer4[4];
unsigned char buffer2[2];


char* seconds_to_time(float seconds);
/*
int main(){
    char *filename = "COIN.wav";
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL){
        printf("I AM SORRY THE FILE CANNOT BE OPENED");
        return 0; 
    }

    header_t h = getHeader(fp);
    filedata_t d = getfileData(fp, h);
    fclose(fp);

    fp = fopen("showingOff.wav", "wb");

    writeFile(fp, h, d);


    fclose(fp);

    freeFileData(d);

    return 0;
}*/

header_t getHeader(FILE *fp) {


    header_t header;

    int read = 0;

    // read header parts

    read = fread(header.riff, sizeof(header.riff), 1, fp);
    printf("(1-4): %s \n", header.riff);

    read = fread(buffer4, sizeof(buffer4), 1, fp);
    printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    // convert little endian to big endian 4 byte int
    header.overall_size = buffer4[0] |
                          (buffer4[1] << 8) |
                          (buffer4[2] << 16) |
                          (buffer4[3] << 24);

    printf("(5-8) Overall size: bytes:%u, Kb:%u n\n", header.overall_size, header.overall_size / 1024);

    read = fread(header.wave, sizeof(header.wave), 1, fp);
    printf("(9-12) Wave marker: %s\n", header.wave);

    read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, fp);
    printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

    read = fread(buffer4, sizeof(buffer4), 1, fp);
    printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    // convert little endian to big endian 4 byte integer
    header.length_of_fmt = buffer4[0] |
                           (buffer4[1] << 8) |
                           (buffer4[2] << 16) |
                           (buffer4[3] << 24);
    printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

    read = fread(buffer2, sizeof(buffer2), 1, fp);
    printf("%x %x \n", buffer2[0], buffer2[1]);

    header.format_type = buffer2[0] | (buffer2[1] << 8);
    char format_name[10] = "";
    if (header.format_type == 1)
        strcpy(format_name, "PCM");
    else if (header.format_type == 6)
        strcpy(format_name, "A-law");
    else if (header.format_type == 7)
        strcpy(format_name, "Mu-law");

    printf("(21-22) Format type: %u %s \n", header.format_type, format_name);

    read = fread(buffer2, sizeof(buffer2), 1, fp);
    printf("%x %x \n", buffer2[0], buffer2[1]);

    header.channels = buffer2[0] | (buffer2[1] << 8);
    printf("(23-24) Channels: %u \n", header.channels);

    read = fread(buffer4, sizeof(buffer4), 1, fp);
    printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    header.sample_rate = buffer4[0] |
                         (buffer4[1] << 8) |
                         (buffer4[2] << 16) |
                         (buffer4[3] << 24);

    printf("(25-28) Sample rate: %u\n", header.sample_rate);

    read = fread(buffer4, sizeof(buffer4), 1, fp);
    printf("%x %x %x %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

    header.byterate = buffer4[0] |
                      (buffer4[1] << 8) |
                      (buffer4[2] << 16) |
                      (buffer4[3] << 24);
    printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate * 8);

    read = fread(buffer2, sizeof(buffer2), 1, fp);
    printf("%x %x \n", buffer2[0], buffer2[1]);

    header.block_align = buffer2[0] |
                         (buffer2[1] << 8);
    printf("(33-34) Block Alignment: %u \n", header.block_align);

    read = fread(buffer2, sizeof(buffer2), 1, fp);
    printf("%x %x \n", buffer2[0], buffer2[1]);

    header.bits_per_sample = buffer2[0] |
                             (buffer2[1] << 8);
    printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);

    header.data_chunk_header[0] = 0;
    header.data_chunk_header[1] = 0;
    header.data_chunk_header[2] = 0;
    header.data_chunk_header[3] = 0;
    header.data_chunk_header[4] = 0;

    header.data_size =0;

    while (strcmp(header.data_chunk_header, "data") != 0){
        fseek(fp, header.data_size, SEEK_CUR);
        read = fread(header.data_chunk_header, sizeof(header.data_chunk_header) - 1, 1, fp);
        printf("(37-40) Data Marker: %s \n", header.data_chunk_header);

        read = fread(buffer4, sizeof(buffer4), 1, fp);
        printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

        header.data_size = buffer4[0] |
                           (buffer4[1] << 8) |
                           (buffer4[2] << 16) |
                           (buffer4[3] << 24);
        printf("(41-44) Size of data chunk: %u \n", header.data_size);

        if (strcmp(header.data_chunk_header, "data") != 0){
            header.overall_size -= 8 + header.data_size;
        }

    }





    // calculate no.of samples
    long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
    printf("Number of samples:%lu \n", num_samples);

    long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
    printf("Size of each sample:%ld bytes\n", size_of_each_sample);

    // calculate duration of file
    float duration_in_seconds = (float) header.overall_size / header.byterate;
    printf("Approx.Duration in seconds=%f\n", duration_in_seconds);
    printf("Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));

    return header;

}

filedata_t getfileData(FILE *fp, header_t header){

    filedata_t data = {.noChannels = header.channels};

    // read each sample from data chunk if PCM
    if (header.format_type == 1) { // PCM
        long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
        long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
        long i =0;
        int read = 0;
        // ALLOC DATA FOR THE CHANNEL

        data.channels = malloc(header.channels * sizeof(channel_t *));

        char data_buffer[size_of_each_sample];
        int  size_is_correct = TRUE;

        // make sure that the bytes-per-sample is completely divisible by num.of channels
        long bytes_in_each_channel = (size_of_each_sample / header.channels);
        if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
            printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
            size_is_correct = FALSE;
        }

        if (size_is_correct) {
            // the valid amplitude range for values based on the bits per sample
            long low_limit = 0l;
            long high_limit = 0l;

            switch (header.bits_per_sample) {
                case 8:
                    low_limit = -128;
                    high_limit = 127;
                    break;
                case 16:
                    low_limit = -32768;
                    high_limit = 32767;
                    break;
                case 32:
                    low_limit = -2147483648;
                    high_limit = 2147483647;
                    break;
            }

            for (int i = 0 ; i < header.channels; i ++){
                data.channels[i].size = num_samples;
                data.channels[i].samples = malloc(sizeof (unsigned int) * num_samples);
            }

            printf("nn.Valid range for data values : %ld to %ld \n", low_limit, high_limit);
            for (i =0; i < num_samples; i++) {

                //printf("==========Sample %ld / %ld=============\n", i, num_samples);
                //printf("1\n");
                read = fread(data_buffer, sizeof(data_buffer), 1, fp);
                if (read == 1) {

                    // dump the data read
                    unsigned int  xchannels = 0;
                    int data_in_channel = 0;
                    int offset = 0; // move the offset for every iteration in the loop below
                    for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {



                        //printf("Channel#%d : ", (xchannels+1));

                        // convert data from little endian to big endian based on bytes in each channel sample
                        if (bytes_in_each_channel == 4) {
                            data_in_channel = (data_buffer[offset] & 0x00ff) |
                                              ((data_buffer[offset + 1] & 0x00ff) <<8) |
                                              ((data_buffer[offset + 2] & 0x00ff) <<16) |
                                              (data_buffer[offset + 3]<<24);
                        }
                        else if (bytes_in_each_channel == 2) {
                            data_in_channel = (data_buffer[offset] & 0x00ff) |
                                              (data_buffer[offset + 1] << 8);
                        }
                        else if (bytes_in_each_channel == 1) {
                            data_in_channel = data_buffer[offset] & 0x00ff;
                            data_in_channel -= 128; //in wave, 8-bit are unsigned, so shifting to signed
                        }

                        offset += bytes_in_each_channel;
                        //printf("%d ", data_in_channel);
                        data.channels[xchannels].samples[i] = data_in_channel;

                        // check if value was in range
                        if (data_in_channel < low_limit || data_in_channel > high_limit)
                            printf("**value out of range\n");

                        //printf(" | ");
                    }

                    //printf("\n");
                }
                else {
                    printf("Error reading file. %d bytes\n", read);
                    break;
                }

            } // 	for (i =1; i <= num_samples; i++) {

        } // 	if (size_is_correct) {
    } else {
        printf("Data not read\n");
    }//  if (header.format_type == 1) {

    printf("Exiting Function..\n");

    // cleanup before quitting
    return data;

}

void freeFileData(filedata_t f){
    for (int i = 0; i < f.noChannels; i ++){
        free(f.channels[i].samples);
    }
    free(f.channels);
}

/**
 * Convert seconds into hh:mm:ss format
 * Params:
 *	seconds - seconds value
 * Returns: hms - formatted string
 **/
char* seconds_to_time(float raw_seconds) {
    char *hms;
    int hours, hours_residue, minutes, seconds, milliseconds;
    hms = (char*) malloc(100);

    sprintf(hms, "%f", raw_seconds);

    hours = (int) raw_seconds/3600;
    hours_residue = (int) raw_seconds % 3600;
    minutes = hours_residue/60;
    seconds = hours_residue % 60;
    milliseconds = 0;

    // get the decimal part of raw_seconds to get milliseconds
    char *pos;
    pos = strchr(hms, '.');
    int ipos = (int) (pos - hms);
    char decimalpart[15];
    memset(decimalpart, ' ', sizeof(decimalpart));
    strncpy(decimalpart, &hms[ipos+1], 3);
    milliseconds = atoi(decimalpart);


    sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
    return hms;
}

void writeFile(FILE *fp, header_t header, filedata_t data){

    // RIFF
    fwrite(header.riff, sizeof (header.riff), 1, fp );
    printf("Writen the RIFF : %s\n", header.riff);

    //OVERALL SIZE
    buffer4[0] = header.overall_size & (0xFF);
    buffer4[1] = (header.overall_size >> 8) & (0xFF);
    buffer4[2] = (header.overall_size >> 16) & (0xFF);
    buffer4[3] = (header.overall_size >> 24) & (0xFF);

    printf("OVERALL SIZE : %x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    fwrite(buffer4, sizeof(buffer4), 1, fp);

    //HEADER WAVE
    fwrite(header.wave, sizeof (header.wave), 1, fp );
    printf("Writen the WAVE : %s\n", header.wave);

    fwrite(header.fmt_chunk_marker, sizeof (header.fmt_chunk_marker) , 1, fp );
    printf("Writen the WAVE : %s\n", header.fmt_chunk_marker);

    //LENGTH OF FMT
    buffer4[0] = header.length_of_fmt & (0xFF);
    buffer4[1] = (header.length_of_fmt >> 8) & (0xFF);
    buffer4[2] = (header.length_of_fmt >> 16) & (0xFF);
    buffer4[3] = (header.length_of_fmt >> 24) & (0xFF);

    printf("Length of FMT : %x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    fwrite(buffer4, sizeof(buffer4), 1, fp);

    //FORMAT TYPE
    buffer2[0] = header.format_type & (0xFF);
    buffer2[1] = (header.format_type >> 8) & (0xFF);

    printf("FORMAT TYPE : %x %x \n", buffer2[0], buffer2[1]);
    fwrite(buffer2, sizeof(buffer2), 1, fp);

    //CHANNELS
    buffer2[0] = header.channels & (0xFF);
    buffer2[1] = (header.channels >> 8) & (0xFF);

    printf("CHANNELS : %x %x \n", buffer2[0], buffer2[1]);
    fwrite(buffer2, sizeof(buffer2), 1, fp);

    //SAMPLE RATE
    buffer4[0] = header.sample_rate & (0xFF);
    buffer4[1] = (header.sample_rate >> 8) & (0xFF);
    buffer4[2] = (header.sample_rate >> 16) & (0xFF);
    buffer4[3] = (header.sample_rate >> 24) & (0xFF);

    printf("Sample Rate : %x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    fwrite(buffer4, sizeof(buffer4), 1, fp);

    //BYTE RATE
    buffer4[0] = header.byterate & (0xFF);
    buffer4[1] = (header.byterate >> 8) & (0xFF);
    buffer4[2] = (header.byterate >> 16) & (0xFF);
    buffer4[3] = (header.byterate >> 24) & (0xFF);

    printf("BYTE RATE : %x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    fwrite(buffer4, sizeof(buffer4), 1, fp);

    //BLOCK ALIGNMENT
    buffer2[0] = header.block_align & (0xFF);
    buffer2[1] = (header.block_align >> 8) & (0xFF);

    printf("Block alignment : %x %x \n", buffer2[0], buffer2[1]);
    fwrite(buffer2, sizeof(buffer2), 1, fp);

    //bits per sample
    buffer2[0] = header.bits_per_sample & (0xFF);
    buffer2[1] = (header.bits_per_sample >> 8) & (0xFF);

    printf("bits per sample : %x %x \n", buffer2[0], buffer2[1]);
    fwrite(buffer2, sizeof(buffer2), 1, fp);

    // data header
    fwrite(header.data_chunk_header, sizeof (header.data_chunk_header) - 1, 1, fp );
    printf("Writen the DATA HEADER : %s\n", header.data_chunk_header);

    //data size
    buffer4[0] = header.data_size & (0xFF);
    buffer4[1] = (header.data_size >> 8) & (0xFF);
    buffer4[2] = (header.data_size >> 16) & (0xFF);
    buffer4[3] = (header.data_size >> 24) & (0xFF);

    printf("Data size : %x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
    fwrite(buffer4, sizeof(buffer4), 1, fp);

    long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
    char data_buffer[size_of_each_sample];
    long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);

    long bytes_in_each_channel = (size_of_each_sample / header.channels);
    if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
        printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
        return;
    }

    long low_limit = 0l;
    long high_limit = 0l;

    switch (header.bits_per_sample) {
        case 8:
            low_limit = -128;
            high_limit = 127;
            break;
        case 16:
            low_limit = -32768;
            high_limit = 32767;
            break;
        case 32:
            low_limit = -2147483648;
            high_limit = 2147483647;
            break;
    }

    for (int i = 0; i < num_samples; i ++){

        // dump the data read
        unsigned int  xchannels = 0;
        int data_in_channel = 0;
        int offset = 0; // move the offset for every iteration in the loop below
        for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {


            //printf("Channel#%d : ", (xchannels+1));

            // convert data from little endian to big endian based on bytes in each channel sample
            if (bytes_in_each_channel == 4) {
                data_buffer[offset] = data.channels[xchannels].samples[i] & 0xFF ;
                data_buffer[offset + 1] = (data.channels[xchannels].samples[i] >> 8) & 0xFF ;
                data_buffer[offset + 2] = (data.channels[xchannels].samples[i] >> 16) & 0xFF ;
                data_buffer[offset + 3] = (data.channels[xchannels].samples[i] >> 24) & 0xFF ;
            }
            else if (bytes_in_each_channel == 2) {
                data_buffer[offset] = data.channels[xchannels].samples[i] & 0xFF ;
                data_buffer[offset + 1] = (data.channels[xchannels].samples[i] >> 8) & 0xFF ;
            }
            else if (bytes_in_each_channel == 1) {
                data_buffer[offset] = (data.channels[xchannels].samples[i] + 128) & 0xFF ;
            }

            offset += bytes_in_each_channel;
            //printf("%d ", data_in_channel);
            // check if value was in range
            if (data_in_channel < low_limit || data_in_channel > high_limit)
                printf("**value out of range\n");

            //printf(" | ");
        }

        fwrite(data_buffer, sizeof(data_buffer), 1, fp);

    }


}



