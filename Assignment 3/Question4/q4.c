

#include <stdio.h>

int main()
{
    int n;

    // Prompt the user to enter an odd value for N
    printf("Enter an odd value for N: ");
    if (scanf("%d", &n) != 1 || n < 3 || n % 2 == 0 || n > 23)
    {
        printf("Please enter a valid odd integer between 3 and 23.\n");
        return 1;
    }

    int rows = 2 * n - 1;
    int cols = 4 * n - 3;
    int cx = 2 * (n - 1); // Horizontal center column

    // 2D character array  enough to support N up to 23 (including N = 21)
    char grid[50][100];

    //  Initialize the  2D grid with spaces
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            grid[y][x] = ' ';
        }
    }

    //   the upper half of the pentagon (rows 0 to n - 1)
    // Each row expands outward by 2 columns from the center
    for (int y = 0; y < n; y++)
    {
        int offset = 2 * y;
        grid[y][cx - offset] = '*';
        grid[y][cx + offset] = '*';
    }

    // Fill the lower half of the pentagon (rows n to 2*n - 2)
    for (int step = 1; step < n; step++)
    {
        int row = (n - 1) + step;

        if (step < n - 1)
        {
            // Lower slanted sides taper inward by 1 column per row
            int offset = 2 * (n - 1) - step;
            grid[row][cx - offset] = '*'; // cx - off set se left side pr move karta ha 
            grid[row][cx + offset] = '*';  // cx + offset se right side pr move karta ha 
        }
        else
        {
            // Bottom horizontal base: N stars spaced 2 columns apart
            int baseHalf = n - 1;
            for (int k = -baseHalf; k <= baseHalf; k += 2)
            {
                grid[row][cx + k] = '*';
            }
        }
    }

     //Print the completed pentagon pattern from the 2D array
    printf("\n");
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }

    return 0;
}