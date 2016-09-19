#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //checking to make sure they provided exactly two command-line arguments
    if (argc != 2) {
        printf("You failed to provide exactly two command-line arguments.\n");
        return 1; //if they don't, return a 1
    }
    
    //checking to make sure they provided a valid, alphabetical keyword
    string key = argv[1];
    int keyLength = strlen(key);
    for (int i = 0; i < keyLength; i++)
    {
        char currentChar = key[i];
        int j = (int) currentChar;
        if (j < 65 || j > 122 || (j > 90 && j < 97))
        {
            printf("You failed to provide a valid keyword.\n");
            return 1;
        }
    }
    
    //Requesting input for the plaintext from the user
    printf("plaintext: ");
    string userInput = get_string();
    int length = strlen(userInput);
    
    //Formatting for the output
    printf("ciphertext: ");
    
    int keywordIndex = 0; //setting the initial keyword index
    bool timeToLoop = false; //boolean that will keep track of when to loop
    
    //
    for (int i = 0; i < length; i++) //go through the entire string, character by character
    {
        char currentChar = userInput[i]; //retrieving the current character from the string
        int j = (int) currentChar; //converting the current character to ASCII
        
        if ((j > 64 && j < 91) || (j > 96 && j < 123)) //check to see if the current character is a letter
        {
            //code for enciphering letters
            
            if (keywordIndex == keyLength) //checking to see if it should be time to loop
            {
                timeToLoop = true;
            }
            
            //Upper Case Enciphering
            if (j > 64 && j < 91)
            {
                //First, check to make sure it isn't time to loop the keyword index
                if (timeToLoop == true)
                {
                    keywordIndex = 0; //wrapping back around
                    timeToLoop = false; //reset the boolean to restart the keyword place counter
                }

                //code for enciphering upper case letters in the plaintext
                
                int alphaOfChar = j - 65; //converting the character to alphabetical (change of -97 for lower case)
                char currentKeywordChar = key[keywordIndex]; //grabbing the current character for the keyword
                int asciiOfKeywordChar = (int) currentKeywordChar;
                
                if (islower(asciiOfKeywordChar) == 0) //if the current keyword character is UPPER CASE
                {
                    asciiOfKeywordChar = tolower(asciiOfKeywordChar); //converting upper case to lower case because it doesn't matter
                }
                int alphaOfKeywordChar = asciiOfKeywordChar - 97;
                
                //Now that the keyword's current letter is definitely lower case, we can shift the current plaintext character by the keyword character
                alphaOfChar = alphaOfChar + alphaOfKeywordChar; //shifting the plaintext character by they keyword character
                alphaOfChar = alphaOfChar % 26; //making sure to account for going past 'z' on the shift
                
                keywordIndex++; //add one to the current keywordIndex
                
                alphaOfChar = alphaOfChar + 65; //shifting back to ascii
                printf("%c", (char) alphaOfChar); //print the modified character
            }
            
            
            //Lower Case Enciphering
            if (j > 96 && j < 123)
            {
                //First, check to make sure it isn't time to loop the keyword index
                if (timeToLoop == true)
                {
                    keywordIndex = 0;
                    timeToLoop = false; //now that the keyword has looped, I don't want it to think it's at the beginning every time. so now reset the boolean.
                }

                //code for enciphering upper case letters in the plaintext
                
                int alphaOfChar = j - 97; //converting the character to alphabetical (change of -97 for lower case)
                char currentKeywordChar = key[keywordIndex]; //grabbing the current character for the keyword
                int asciiOfKeywordChar = (int) currentKeywordChar;
                
                if (islower(asciiOfKeywordChar) == 0) //if the current keyword character is UPPER CASE
                {
                    asciiOfKeywordChar = tolower(asciiOfKeywordChar); //converting upper case to lower case because it doesn't matter
                }
                
                int alphaOfKeywordChar = asciiOfKeywordChar - 97;
                
                //Now that the keyword's current letter is definitely lower case, we can shift the current plaintext character by the keyword character
                alphaOfChar = alphaOfChar + alphaOfKeywordChar; //shifting the plaintext character by they keyword character
                alphaOfChar = alphaOfChar % 26; //making sure to account for going past 'z' on the shift
                
                keywordIndex++; //add one to the current keywordIndex
                
                alphaOfChar = alphaOfChar + 97; //shifting back to ascii
                printf("%c", (char) alphaOfChar); //print the modified character
            }
        }
        
        else
        {
            printf("%c", currentChar);
        }
    }
    printf("\n"); //formatting!
    return 0; //return 0 after outputting cipher text
}
//Big shoutout to the blessed Harambe for making this code work