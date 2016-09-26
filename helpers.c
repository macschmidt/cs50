
/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

bool search(int value, int values[], int n);
bool newSearch(int value, int values[], int n);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int length = n; // just for clarity purposes
    int startIndex = 0;
    int endIndex = length - 1; // because arrays are only 0 --> length - 1, not length
    int middleIndex = (startIndex + endIndex) / 2; // first guess!
    
    while (endIndex >= startIndex) // hopefully, this means the call to newSearch should last until it's thoroughly searched the array of ints
    {
        if (values[middleIndex] == value) // returns true if it finds the value at the location we're looking for
        {
            return true;
        }
    
        else if (values[middleIndex] > value) // if the current number is greater than the value, then search the left half
        {
            endIndex = middleIndex - 1; // to search the left half, set the end index to that middle index (start index stays at old value)
            middleIndex = (startIndex + endIndex) / 2; // you have to recalculate the middle index because otherwise you're just going to be searching from the same place over and over again
        }
    
        else if (values[middleIndex] < value) // if the current number is less than the value, then search through the right half
        {
            startIndex = middleIndex + 1; // to search the right half, set the start index to that middle index (end index stays at old value)
            middleIndex = (startIndex + endIndex) / 2; // see above for why we do this
        }
        // it is going to run again because endIndex is still greater than or equal to startIndex (unless it's not obviously, then it returns false)
    }
    return false; // if it goes through and gets to a point where the endIndex is less than (behind) the startIndex, you have failed to find the number
}

/**
 * Sorts array of n values.
 */
 
void sort(int values[], int n)
{
    int countingArray[65536];
    
    for (int i = 0; i < 65536; i++) // just filling an array full of 0s
    {
        countingArray[i] = 0;
    }
    
    int currentVal = 0;
    
    for (int i = 0; i < n; i++) // go through the entire values[] array of unsorted numbers
    {
        currentVal = values[i]; // take the value at the current location in the values array
        countingArray[currentVal]++; // add a counter of one to the countingArray array
    }
    
    // currently, I have an array that for each possible value, holds a counter of how many times each possible value was held
    
    int locationInOriginalArray = 0;
    
    for (int currentRandomNum = 0; currentRandomNum < 65536; currentRandomNum++) // go through the entire array of possible values
    {
        while (countingArray[currentRandomNum] != 0) // if there is a counter for a specific value
        {
            values[locationInOriginalArray] = currentRandomNum; // then add that value to the values[] array
            countingArray[currentRandomNum]--; // decrease the counter at that value by one (there can still be more!)
            locationInOriginalArray++;
        }
    }
}