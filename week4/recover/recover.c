#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

// check that input is only 1 argmument
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int BLOCK_SIZE = 512;
    unsigned char buffer[BLOCK_SIZE];
    int imageCount = 0;
    int headerCount = 0;
    char* fileName = malloc(8);
    FILE *img;
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff & buffer[1] == 0xd8 & buffer[2] == 0xff & (buffer[3] & 0xf0) == 0xe0)
        {
            if (headerCount == 0)
            {
                // open file with new name
                sprintf(fileName, "%03i.jpg", headerCount);
                img = fopen(fileName, "w");
                // write to file
                fwrite(buffer, 1, BLOCK_SIZE, img);
                headerCount++;
            }

            else
            {
                // close previous file
                fclose(img);
                // change filename
                sprintf(fileName, "%03i.jpg", headerCount);
                img = fopen(fileName, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
                headerCount++;
            }
        }
        else if (headerCount > 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }
    // close the last image
    fclose(img);
    // close the raw file
    fclose(raw_file);
    free(fileName);
}