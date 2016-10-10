/**
 * Implements a dictionary's functionality.
 */
 
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

// this is some arbitrarily large number to make sure we have enough room. If I used a grow function, this would not be constant.
#define HASHSIZE 5000000

// this will be used for number of words in the given dictionary
int dictionarySize = 0;

// Creating the struct for nodes, which will hold the strings for the dictionary
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Creating my hash table for the dictionary and the load function

// filling the hash table with NULL
node *hashTable[HASHSIZE] = {NULL};

// hash function
int hash(char* element) 
{
    int hash = 0;
    int tempNum = 0;
    
    for (int i = 0, length = strlen(element); i < length; i++)
    {
        // if the current character is an alphabetical letter, give it a number through 26
        // this is going to be it's "bucket"
        if (isalpha(element[i]) == true)
        {
            // turning the character into an integer value through '0'
            tempNum = element[i] - '0';
        }
        
        // the only other case for a character is it being a comma, so we'll give commas 27
        else
        {
            tempNum = 27;
        }
        
        // giving the hashkey it's own unique bucket, based on its letter
        hash = hash + tempNum;
    }
    return hash;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    
    // first, take whatever the word is and turn it into a lower-case word for simplicity sake
    char lowerCaseWord[LENGTH + 1];
    int length = strlen(word);
    
    // converting each letter in the char array (a string) to lower case
    for (int i = 0; i < length; i++)
    {
        lowerCaseWord[i] = tolower(word[i]);
    }
    
    // this line is necessary because otherwise we don't have that last character to look for, and we need that to show where
    // one word ends and another begins
    lowerCaseWord[length] = '\0';
    
    // find the location in memory of this word
    int hashkey = hash(lowerCaseWord);
    
    // next, check to see if the hashTable at this location is empty
    if (!hashTable[hashkey])
    {
        return false;
    }
    
    // CREDIT TO ZAMALYA AND THE YOUTUBE VIDEO FOR THE CODE BETWEEN THESE TWO LINES OF COMMENTS
    //////////////////////////////////////////////////////////////////////////////////////////
    // now, create a node pointer which will be used for comparison
    node* cursor = hashTable[hashkey];
    
    // we now know that the hashtable is not empty at the index, so we need to compare words
    while (cursor != NULL)
    {
        if (strcmp(lowerCaseWord, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    
    // this is the default case, in case the program doesn't return true (meaning the word isn't found)
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    
    // int that represents the current element's hashkey
    // int hashkey = -1;
    
    // Open the dictionary file with reading permissions
    FILE* dictionaryFile = fopen(dictionary, "r");
    
    // Check to make sure that you could open the dictionary
    if (!dictionaryFile)
    {
        printf("Could not open dictionary file %s\n", dictionary);
        return false;
    }
    
    // array for the word, +2 because you have to take into account \0
    char word[LENGTH + 1];
    
    // Go through the dictionary file, setting the current string to a variable called "word" and put into the hash table
    while (fscanf(dictionaryFile, "%s\n", word) != EOF)
    {
        // because we have a word, add one to the dictionary size!
        dictionarySize++;
        
        // creating a new node with the correct size in memory
        node *new_node = malloc(sizeof(node));
        
        // if we run out of memory (malloc returns NULL), return false and unload the memory
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        
        // Copying the word into the new node
        strcpy(new_node->word, word);
        
        // get hashkey, which is the index of a bucket in the hash table
        int hashkey = hash(word);
        
        // WE HAVE TO MOD THE HASHKEY TO MAKE SURE WE STAY IN BOUNDS!!!!
        hashkey = hashkey % HASHSIZE;
        
        // if the hashtable is empty at the hashkey, put the word in
        if (hashTable[hashkey] == NULL)
        {
            hashTable[hashkey] = new_node;
            new_node->next = NULL;
        }
        
        // if the hashtable has something at the hashkey, add the word to the end of the linked list
        else
        {
            new_node->next = hashTable[hashkey];
            hashTable[hashkey] = new_node;
        }
    }
    
    // tidying stuff up
    fclose(dictionaryFile);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    // if there are words loaded into the dictionary, then return the size of that (the number of words loaded)
    if (dictionarySize > 0)
    {
        return dictionarySize;
    }
    
    // otherwise, return 0 to show that the dictionary is not yet loaded
    else {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    
    // I'm going to use this to go through the entire hash function with a while loop
    int hashLocation = 0;
    
    // run through the whole hashtable, and continuing running until you reach the end
    while (hashLocation < HASHSIZE)
    {
        // if the hashtable is empty at the current hashLocation, then move to the next hashLocation (add one to hashLocation)
        if (hashTable[hashLocation] == NULL)
        {
            hashLocation++;
        }
        
        // if the hashtable is NOT empty, however, you need to free the memory at each of the different nodes that stem off of that
        // index
        else
        {
            // while there is something in this location in memory...
            while (hashTable[hashLocation] != NULL)
            {
                // create a node called cursor to this location in memory
                node* cursor = hashTable[hashLocation];
                // set this location in memory to the next node, thereby shifting
                hashTable[hashLocation] = cursor->next;
                // and free the memory at the old location in memory
                free(cursor);
            }
            
            // now that that memory is freed up, increase the hashLocation by one
            hashLocation++;
        }
    }
    
    // the case here is true because if we get through the memory and free it, we want to return true, not false
    return true;
}