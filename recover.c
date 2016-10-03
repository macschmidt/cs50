#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE; // use this later in the array of bytes called currentJPEGSearch of size 512

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n"); // using stderr here for the error output
        return 1;
    }
    
    FILE* inputFile = fopen("card.raw", "r"); // opening the input file, card.raw
    FILE* outputFile = NULL; // creating an empty output file
    
    // check to see if you can open the input file
    if (inputFile == NULL)
    {
        fprintf(stderr, "Could not open the file.\n"); // using stderr here for the error output
        return 2;
    }
    
    // this is the character array that stores the resultant string for the name of the jpeg
    // it is of size 7 because the name of the file is only ever going to be as long as "999.jpg", which is 7 characters
    char fileName[7];
    
    // this is an integer that will keep track of how many JPEGs we have found
    int numberJPEG = 0;
    
    // I understand that this works as a buffer, but it makes more logical sense to me to call it
    // currentJPEGSearch because when I look at currentJPEGSearch[0], it makes sense because I am looking
    // at the '0'th byte of the currentJPEG. Sure, when you don't have a JPEG it doesn't work the same
    // way, but that's why I called it currentJPEGSearch instead of currentJPEG (I know it's not the
    // shortest name, but it helps me. Hopefully anyone reading this will now understand the logic behind
    // my variable naming).
    BYTE currentJPEGSearch[512]; 
    
    while (feof(inputFile) == 0) // while we are NOT at the end of the file, run through the memory card (inputFile)
    {
        // Check to see if the first three bytes of the file are the JPEG header (0xff, 0xd8, and 0xe0) and also
        // check (currentJPEGSearch[3] & 0xf0) == 0xe0). To be honest, I don't fully understand this last bit of
        // syntax, but it was recommended as the parameters for checking if there is a JPEG, so here it is.
        if (currentJPEGSearch[0] == 0xff && currentJPEGSearch[1] == 0xd8 && currentJPEGSearch[2] == 0xff && (currentJPEGSearch[3] & 0xf0) == 0xe0) // major credit to Zamalya for this code
        {
            // Give the JPEG an identifying name based on the order we found it in
            sprintf(fileName, "%03i.jpg", numberJPEG);
            
            // setting the output file to the JPEG that we just found, given writing permissions
            outputFile = fopen(fileName, "w");
            
            // because we have now successfully found a file, we can increase the number of JPEGS we have found by 1! Hooray!
            numberJPEG++;
            
            // wrtiing the bytes at currentJPEGSearch into the outputFile
            fwrite(currentJPEGSearch, sizeof(currentJPEGSearch), 1, outputFile); // Go 512 bytes at a time here because we have the JPEG
        }
        
        else if (numberJPEG > 0) // if we aren't at a JPEG (because the above if-statement wasn't true) but we do have JPEGS found, then...
        {
            // write the JPEG into the output file!
            fwrite(currentJPEGSearch, sizeof(currentJPEGSearch), 1, outputFile);            
        }
        
        // I was about to put an else statement here just for my own logical sense, but I remember from the comments from last
        // week that it would be pointless to include it – only thought to put it in to emphasize the logic to myself while 
        // working on this.
        
        // if the numberJPEG is zero (aka we we don't have any more JPEGs), then read in the next 512 bytes from the input file
        fread(currentJPEGSearch, sizeof(currentJPEGSearch), 1, inputFile);
    }
    
    fclose(inputFile); // close the file
    
    return 0; // successful run means we return 0, as per the spec
}