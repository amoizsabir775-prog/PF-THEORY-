#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Please provide the input filename.\n");
        return 1;
    }

    FILE *file_ptr = fopen(argv[1], "r");
    if (file_ptr == NULL) {
        printf("Error! File is not open\n");
        return 1;
    }

    int N, G;
    fscanf(file_ptr, "%d", &N);
    fscanf(file_ptr, "%d", &G);

    int **curr_grid = (int **)malloc(N * sizeof(int *));
    int **next_grid = (int **)malloc(N * sizeof(int *));

    int i, j;
    for (i = 0; i < N; i++) {
        curr_grid[i] = (int *)malloc(N * sizeof(int));
        next_grid[i] = (int *)malloc(N * sizeof(int));
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fscanf(file_ptr, "%d", &curr_grid[i][j]);
            next_grid[i][j] = 0;
        }
    }

    fclose(file_ptr);

    int gen;
    for (gen = 0; gen < G; gen++) {
        int row, col;
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                int live_neighbors = 0;
                int dr, dc;

                for (dr = -1; dr <= 1; dr++) {
                    for (dc = -1; dc <= 1; dc++) {
                        if (dr == 0 && dc == 0) {
                            continue;
                        }
                        int nr = row + dr;
                        int nc = col + dc;

                        if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                            if (curr_grid[nr][nc] == 1) {
                                live_neighbors++;
                            }
                        }
                    }
                }

                if (curr_grid[row][col] == 1) {
                    if (live_neighbors < 2 || live_neighbors > 3) {
                        next_grid[row][col] = 0;
                    } else {
                        next_grid[row][col] = 1;
                    }
                } else {
                    if (live_neighbors == 3) {
                        next_grid[row][col] = 1;
                    } else {
                        next_grid[row][col] = 0;
                    }
                }
            }
        }

        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                curr_grid[row][col] = next_grid[row][col];
            }
        }
    }

    printf("Final Grid after %d generations:\n", G);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", curr_grid[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < N; i++) {
        free(curr_grid[i]);
        free(next_grid[i]);
    }
    free(curr_grid);
    free(next_grid);

    return 0;
}
