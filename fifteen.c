/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(100000);
        }

        // sleep thread for animation's sake
        usleep(100000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(100000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int currentVal = (d*d) - 1; // top left hand value
    
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            board[row][column] = currentVal;
            currentVal--; // decrease by one because each square advanced is going to be one less
        }
    }
    
    if (d % 2 == 0) // if the dimension is even
    {
        // swap the '1' and the '2' tiles (special case)
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            if (board[row][column] == 0) // print an underscore for where the '0' is
            {
                printf(" _ ");
            }
            else
            {
                printf("%2i ", board[row][column]); // print the current number
            }
        }
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int rowOfTile, columnOfTile;
    int rowOfBlank, columnOfBlank;
    
    // the first step is to find where the tile is that the user has told us to move
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            if (board[row][column] == tile) // finding the tile the user asked for
            {
                rowOfTile = row;
                columnOfTile = column;
            }
            if (board[row][column] == 0) // have to know where the blank tile is in order to switch
            {
                rowOfBlank = row;
                columnOfBlank = column;
            }
        }
    }
    // I now have the location of the tile.
    // move the tile, but only if the move is legal
    if (rowOfTile == rowOfBlank && abs(columnOfTile - columnOfBlank) == 1) // check if the blank and the tile are on the same row and only one column apart
    {
        // move the tile
        board[rowOfTile][columnOfTile] = 0;
        board[rowOfBlank][columnOfBlank] = tile;
        return true;
    }
    
    if (columnOfTile == columnOfBlank && abs(rowOfTile - rowOfBlank) == 1) // check if the blank and the tile are on the same column and only one row apart
    {
        board[rowOfTile][columnOfTile] = 0; // move the tile
        board[rowOfBlank][columnOfBlank] = tile;
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int correctValue = 1; // starting in upper left 
    
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            if (row == d-1 && column == d-1) // if you get to the last value, you've won
            {
                return true;
            }
            
            if (board[row][column] != correctValue)
            {
                return false; // if the current value doesn't correspond to the correct one, return false
            }
            
            else
            {
                correctValue++;
            }
        }
    }
    return false; // default case return false
}