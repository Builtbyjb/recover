#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }
    // Opens the memory card file.
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Failed to read file\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE]; // Creates the buffer.
    char str[8]; // Stores the jpeg files.
    FILE *img = NULL;
    int i = 0; // counter,
    while (fread(buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {

        // Looks for the begining of a JPEG file and writes it to str.
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Creates new filenames starting from 000.jpg
            if (i == 0)
            {
                sprintf(str, "%03i.jpg", i);
                img = fopen(str, "w");
                //
                if (img == NULL)
                {
                    printf("Failed to open file\n");
                    return 1;
                }
                // writes 1byte 512times to the img FILE.
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
                i++;
            }
            else
            {
                fclose(img);
                sprintf(str, "%03i.jpg", i);
                img = fopen(str, "w");
                if (img == NULL)
                {
                    printf("Failed to open file\n");
                    return 1;
                }
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
                i++;
            }
        }
        else if (i != 0) // If a JPEG as already been found,  continues writing to the file.
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
            if (input == 0)
            {
                fclose(img);
                fclose(input);
                return 0;
            }
        }
    }
    fclose(input);
    fclose(img);
    return 0;
}