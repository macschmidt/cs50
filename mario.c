#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        printf("How high would you like the blocks to be? Please choose a number between 0 and 23: ");
        height = get_int();
    }
    while (height > 23 || height < 0);
    
    //Now actually make the pyramid
    
    for (int i = 0; i < height; i++) //For each row of the whole pyramid, gaps included. i is the row number you're on.
    {
        
        //Print the spaces for the left pyramid
        //For a pyramid of height 8...
        //Row 0 will have 7 spaces and 1 hashtag
        //Row 1 will have 6 spaces and 2 hashtags
        //Row 2 will have 5 spaces and 3 hashtags
        //Row i will have (height - (i+1)) spaces
        for (int s = 0; s < (height - (i+1)); s++)
        {
            printf(" ");
        }

        //Print the hashtags for the left pyramid
        //for row 0, print 1 hashtag
        //for row 1, print 2 hashtags
        //for row n, print n+1 hashtags
        for (int h = 0; h < (i+1); h++)
        {
            printf("#");
        }
        
        
        //Print the gap
        printf("  ");
        
        //Print the hashtags for the right pyramid
        for (int h = 0; h < (i+1); h++)
        {
            printf("#");
        }
        
        /*
        //Print the spaces for the right pyramid
        for (int s = 0; s < (height - (i+1)); s++)
        {
            printf(" ");
        }
        */
        
        //End the line by printing a return
        printf("\n");
    }
}