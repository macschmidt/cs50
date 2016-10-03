#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize scale infile outfile\n");
        return 1;
    }

    // set the scale input to a variable named 'scale'
    // remember to use atoi() for converting the string to an integer
    int scale = atoi(argv[1]);
    
    // check to see if the scale is an appropriate value (between 1 and 100 inclusive)
    if (scale < 1 || scale > 100)
    {
        printf("The scale must be a positive integer between 1 and 100, inclusive.\n");
        return 1;
    }
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // create variables for the original file's dimensions
    int oldHeight = bi.biHeight;
    int oldWidth = bi.biWidth;
    
    // recalculating the width and height of the bitmap for the given scale
    // note: do not actually need newHeight, because I can just use bi.biHeight once later
    // (it doesn't get used outside of the iterating through the for loop)
    bi.biHeight *= scale;
    bi.biWidth *= scale;
    
    // calculate padding, both for the original file and the new file
    int oldPadding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // recalculating the size of the image, to put into the header
    bi.biSizeImage = abs(bi.biHeight) * ((bi.biWidth * sizeof (RGBTRIPLE)) + newPadding);
    
    // recalculating the size of the file, to put into the header
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // I am going to use the malloc() function to have a set amount of room in storage (length of the line * sizeof(RGBTRIPLE)
    // Why is it this amount of room?
    // I am going to need the number of bytes for the line based on:
        // how long the line is, in pixels (aka bi.biWidth)
        // how large each individual pixel is in bytes
    // So, multiply the number of bytes per byte * the number of pixels on the line
    // malloc() returns a pointer to the allocated memory for that kind of variable, so I need to create a variable of type
    // RGBTRIPLE because that's what it's going to contain
    RGBTRIPLE *placeholder = malloc(bi.biWidth * sizeof(RGBTRIPLE));

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        int counter = 0; // this will help me keep track of which pixel I am working on for writing the pixel
        
        // iterate over pixels in scanline
        for (int j = 0; j < oldWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            // this for loop will set the placeholder to whatever the pixel is
            // an important thing to note here is that it will do it as many times as the scale requires
            // for example, if the scale is 2, then it will set the placeholder two times for two pixels (because you need to double the size of the image)
            for(int k = 0; k < scale; k++)
            {
                // fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); // I was originally going to use this, but had some weird coding issues. Used malloc() instead.
                // need to set whatever location in my file (noted by the pointer placeholder) to the value of 'triple', which holds the value of the pixel
                *(placeholder + (counter)) = triple;
                
                // increase the counter so that it moves to the next pixel
                counter++;
            }
        }
                    
        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);
        
        // NOTE: this loop is outside of the loop used for each scanline because I am using the setup where I remember the line (using malloc)
        // now I actually need to write the pixels into the output file, using a loop that runs through as many times as the scale requires
        // for example, if the scale is 2, the program will write the line TWO times (because you need to resize VERTICALLY as well as horizontally)
        for (int r = 0; r < scale; r++)
        {
            // writing the line to the output file
            fwrite((placeholder), sizeof(RGBTRIPLE), bi.biWidth, outptr);
            
            // before I write the same line again, however, I need to put padding in so that the line is a multiple of four bytes
            for (int k = 0; k < newPadding; k++)
            {
                // use fputc() and not fwrite() for the 0x00 because the video told me to
                fputc(0x00, outptr);
            }
        }
    }

    // this is necessary because I used malloc(), and otherwise I'm going to have issues
    free(placeholder);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}