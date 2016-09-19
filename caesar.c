#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2) { //checking to make sure they provided exactly two command-line arguments
        printf("You failed to provide exactly two command-line arguments.\n");
        return 1; //if they don't, return a 1
    }
    
    int key = atoi(argv[1]); //converting the second argument string to an int
    key = key % 26; //accounting for having a key larger than 26
    
    printf("plaintext: ");
    string userInput = get_string(); //getting the plaintext from the user
    int length = strlen(userInput); //declaring a variable to save memory
    
    printf("ciphertext: ");
    
    for (int i = 0; i < length; i++) //go through the entire string, character by character
    {
        char currentChar = userInput[i];
        int j = (int) currentChar; //converting the current character to ASCII
        
        //Enciphering lower case letters and printing
        if (j > 96 && j < 123) //if the character is within the lower-case region of the ASCII Chart
        {
            //code for enciphering a lower-case letter
            int alphaOfChar = j - 97; //Converting from ASCII to Alphabetical
            alphaOfChar = alphaOfChar + key; //shifting the key amount
            alphaOfChar = alphaOfChar % 26; //accounting for if the number wraps around the alphabet
            int outputLetter = alphaOfChar + 97; //converting back to ASCII
            char c = (char) outputLetter;
            printf("%c", c); //printing out the ASCII as a regular letter
        }        
        
        //Enciphering upper case letters and printing
        else if (j > 64 && j < 91) //if the character is within the upper-case region of the ASCII Chart
        {
            //code for enciphering an upper-case letter
            int alphaOfChar = j - 65; //converting to alphabetical
            alphaOfChar = alphaOfChar + key; //shifting the key amount
            alphaOfChar = alphaOfChar % 26; //accounting for if the number went over too far
            int outputLetter = alphaOfChar + 65; //converting back to ASCII
            char c = (char) outputLetter;
            printf("%c", c); //printing out the ASCII as a regular letter
        }
        
        //Printing non-letters
        if (j < 65 || j > 122 || (j > 90 && j < 97)) //this just checks to make sure that any other character isn't within the letter regions
        {
            printf("%c", currentChar); //print the character as is
        }
    }
    printf("\n"); //formatting!
    return 0; //return 0 after outputting cipher text
}