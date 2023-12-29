#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    fseek(input, 0, SEEK_SET);
    WAVHEADER header;

    fread(&header.chunkID, 1, 4, input);
    fread(&header.chunkSize, sizeof(DWORD), 1, input);
    fread(&header.format, 1, 4, input);
    fread(&header.subchunk1ID, 1, 4, input);
    fread(&header.subchunk1Size, sizeof(DWORD), 1, input);
    fread(&header.audioFormat, sizeof(WORD), 1, input);
    fread(&header.numChannels, sizeof(WORD), 1, input);
    fread(&header.sampleRate, sizeof(DWORD), 1, input);
    fread(&header.byteRate, sizeof(DWORD), 1, input);
    fread(&header.blockAlign, sizeof(WORD), 1, input);
    fread(&header.bitsPerSample, sizeof(WORD), 1, input);
    fread(&header.subchunk2ID, 1, 4, input);
    fread(&header.subchunk2Size, sizeof(DWORD), 1, input);

    // Use check_format to ensure WAV format
    if (check_format(header) != 0)
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");

    if (output == NULL)
    {
        printf("Couldn't open output file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&header, 1, 44, output);

    // Use get_block_size to calculate size of block
    int blockSize = get_block_size(header);

    // Write reversed audio to file
    fseek(input, 0, SEEK_END);
    int numBlocks = ((ftell(input) - 44) / blockSize);
    BYTE buffer[blockSize];

    for (int i = 0; i < numBlocks; i++)
    {
        fseek(input, (numBlocks - i - 1) * blockSize + 44, SEEK_SET);
        fread(&buffer, 1, blockSize, input);
        fwrite(&buffer, 1, blockSize, output);
    }

    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    char *expected = "WAVE";

    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != expected[i])
        {
            return 1;
        }
    }

    return 0;
}

int get_block_size(WAVHEADER header)
{
    return header.numChannels * (header.bitsPerSample / 8);
}