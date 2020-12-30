//import headers
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //ensure only one command-line argument is read in
    if (argc != 2)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    //open card file
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", infile);
        return 2;
    }

    //create outfile
    char *outfile = malloc(4); //allocate 4byte to the adress (32bit machine = 4bytes)

    //initialize "order" of jpegs
    //incrementing had issue with unused expression
    int *pntOrder = malloc(sizeof(int));
    //derefence pntOrder to have a value
    *pntOrder = 0;

    //initialize isJPEG to false
    int *isJpeg = malloc(sizeof(int));
    *isJpeg = 0;

    //repeat until end of card
    unsigned char buffer[512]; //create buffer with 512 bytes

    //give a name to outfile
    sprintf(outfile, "%03i.jpg", *pntOrder); //convert to string somehow
    //create the outfile
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fprintf(stderr, "Could not create %s\n", outfile);
    }

    //loop over the whole file until end
    while (fread(&buffer, 512, 1, inptr) == 1)
    {

        //start of a new jpeg?
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)

        {
            //yes, start of jpeg
            //has already a JPEG been found
            if (*isJpeg == 1)
            {
                //close or save former file
                fclose(outptr);

                //open new file
                //give a name to outfile
                sprintf(outfile, "%03i.jpg", *pntOrder); //convert to string somehow
                //create the outfile
                *outptr = *fopen(outfile, "w");
                if (outptr == NULL)
                {
                    fprintf(stderr, "Could not create %s\n", outfile);
                }

                //write chunk into new file
                fwrite(&buffer, 512, 1, outptr);

                //increase counter
                (*pntOrder)++;


                //step to the next iteration
                continue;
            }
            //this is the first Jpeg
            else
            {
                //write chunk to file
                fwrite(&buffer, 512, 1, outptr);

                //set flag to true
                *isJpeg = 1;

                //increase counter
                (*pntOrder)++;

                //step to next iteration
                continue;
            }
        }
        //chunk is not the beginning of Jpg
        else
        {
            //chunk is a continuation of a Jpg
            if (*isJpeg == 1)
            {
                //write to file
                fwrite(&buffer, 512, 1, outptr);
            }
            //chunk is not part of a jpg

        }
    }
    //EOF
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}