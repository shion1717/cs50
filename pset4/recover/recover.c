#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        return 1;
    }

    //Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }

    //Initialise variables
    char filename[8];
    unsigned char bytes[512];
    int jpegcounter = -1;

    //Continue reading until end of file
    while (fread(bytes, sizeof(bytes), 1, file) == 1)
    {
        //Checks for jpeg
        if (bytes[0] == 0xff &&
            bytes[1] == 0xd8 &&
            bytes[2] == 0xff &&
            (bytes[3] & 0xf0) == 0xe0)
        {
            //Adds one to jpeg counter to start labelling the files
            jpegcounter++;

            //Format the file name
            sprintf(filename, "%03i.jpg", jpegcounter);

            //Create file in write mode
            FILE *img = fopen(filename, "w");

            //Write data to the file
            fwrite(bytes, 512, 1, img);

            //Close the file
            fclose(img);
        }
        else if (jpegcounter >= 0)
        {
            sprintf(filename, "%03i.jpg", jpegcounter);

            //Opens the file in append mode instead since it is a continuation of the jpeg
            FILE *img = fopen(filename, "a");
            fwrite(bytes, 512, 1, img);
            fclose(img);
        }
    }
    fclose(file);
}
