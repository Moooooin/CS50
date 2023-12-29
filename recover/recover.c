#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCKSIZE = 512;

int main(int argc, char *argv[])
{
    // Check for incorrect usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");

    // Check if file exists
    if (file == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    // Create a bufffer, filename and store number of files created in n
    uint8_t *block = malloc(BLOCKSIZE);
    char filename[8] = "000.jpg";
    int n = 0;

    // Read BLOCKSIZE bytes from file into buffer until the end
    while (fread(block, 1, BLOCKSIZE, file) == BLOCKSIZE)
    {
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {
            // Beginning of jpg found
            // Create new file, update count and write current block into new file
            sprintf(filename, "%03i.jpg", n);
            n++;

            FILE *jpg = fopen(filename, "w+");

            fwrite(block, 1, BLOCKSIZE, jpg);
            fclose(jpg);
        }
        else if (n > 0)
        {
            // Continue writing to jpg
            FILE *jpg = fopen(filename, "a");

            fwrite(block, 1, BLOCKSIZE, jpg);
            fclose(jpg);
        }
    }

    // Close all files and free memory
    fclose(file);
    free(block);
    return 0;
}