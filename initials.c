#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string userName = get_string(); //getting input from the user
    int secondStart = 0; //this is the index for the program to start after the first initial is found
    int length = strlen(userName);

    if (userName != NULL) {
        
        for (int i = 0; i < length; i++) //looking for the first initial
        {
            if (userName[i] != ' ') {
                printf("%c", toupper(userName[i]));
                secondStart = i; //setting the index for the second iteration through the string so it doesn't look at the first initial again
                i = length; //exiting the loop upon this condition being fufilled
            }
        }
        
        for (int j = secondStart; j < length; j++) //go through the string, character by character, starting from after the first initial
        {
            //if there's a space AND the character right after is not a space AND you aren't at the end of the string
            if (userName[j] == ' ' && userName[j+1] != ' ' && (j+1) < length) {
                printf("%c", toupper(userName[j+1])); //print the intial in upper case
            }
        }
        printf("\n"); //printing a new line once the initials are outputted
    }
}