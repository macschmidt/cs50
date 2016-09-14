#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    //Initializing variables
    float change;
    //int coins;
    int centAmount;
    int numOfCoins = 0;
    
    //Input loop with check
    do
    {
        printf("How much change do you have? You can enter it as a decimal (3.75): ");
        change = get_float();
    }
    while (change < 0);
   
    //convert dollars to cents
    change = change * 1000;
    change = change / 10;

    //printf("%.55f\n", change); This was for error checking.
    
    centAmount = change;

    //Check to see if quarters can be used
    while (centAmount >= 25)
    {
        centAmount = centAmount - 25;
        numOfCoins++;
    }
    
    //Check to see if dimes can be used
    while (centAmount >= 10)
    {
        centAmount = centAmount - 10;
        numOfCoins++;
    }
    
    //Check to see if nickels can be used
    while (centAmount >= 5)
    {
        centAmount = centAmount - 5;
        numOfCoins++;
    }
    
    //Check to see if pennies can be used
    while (centAmount >= 1)
    {
        centAmount = centAmount - 1;
        numOfCoins++;
    }
    
    printf("%i\n", numOfCoins);
    
}