#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Enter the average length of your shower: ");
    int length = get_int();
    int bottles = length * 12;
    
    printf("Your shower uses %i bottles of water per shower.\n", bottles);
}