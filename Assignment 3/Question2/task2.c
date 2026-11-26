#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

// Check whether N is a power of 2
int isPowerOfTwo(int n)
{
    return n > 0 && (n & (n - 1)) == 0;
}

// Allocate a 3D matrix using contiguous memory
double ***allocate3D(int n, double **data, double ***rows)
{
    int z, y;

    // Allocate one continuous data block
    *data = calloc((size_t)n * n * n, sizeof(double));

    // Allocate row pointers
    *rows = malloc((size_t)n * n * sizeof(double *));

    // Allocate sheet pointers
    double ***grid = malloc(n * sizeof(double **));

    if (*data == NULL || *rows == NULL || grid == NULL)
    {
        free(*data);
        free(*rows);
        free(grid);

        *data = NULL;
        *rows = NULL;

        return NULL;
    }

    // Connect pointers with the continuous data block
    for (z = 0; z < n; z++)
    {
        grid[z] = *rows + z * n;

        for (y = 0; y < n; y++)
        {
            grid[z][y] = *data + (z * n * n) + (y * n);
        }
    }

    return grid;
}

// Free all memory of a 3D matrix
void free3D(double ***grid, double **rows, double *data)
{
    free(grid);
    free(rows);
    free(data);
}

// Perform 1D Radix-2 FFT
void fft1D(double real[], double imag[], int n)
{
    int i, j, k, len;

    // Bit reversal
    j = 0;

    for (i = 1; i < n; i++)
    {
        int bit = n / 2;

        while (j & bit)
        {
            j ^= bit;
            bit >>= 1;
        }

        j ^= bit;

        if (i < j)
        {
            double temp = real[i];
            real[i] = real[j];
            real[j] = temp;

            temp = imag[i];
            imag[i] = imag[j];
            imag[j] = temp;
        }
    }

    // FFT butterfly operations
    for (len = 2; len <= n; len *= 2)
    {
        double angle = -2.0 * PI / len;

        double wlenReal = cos(angle);
        double wlenImag = sin(angle);

        for (i = 0; i < n; i += len)
        {
            double wReal = 1.0;
            double wImag = 0.0;

            for (k = 0; k < len / 2; k++)
            {
                int even = i + k;
                int odd = i + k + len / 2;

                double oddReal =
                    real[odd] * wReal - imag[odd] * wImag;

                double oddImag =
                    real[odd] * wImag + imag[odd] * wReal;

                double evenReal = real[even];
                double evenImag = imag[even];

                real[even] = evenReal + oddReal;
                imag[even] = evenImag + oddImag;

                real[odd] = evenReal - oddReal;
                imag[odd] = evenImag - oddImag;

                double nextReal =
                    wReal * wlenReal - wImag * wlenImag;

                double nextImag =
                    wReal * wlenImag + wImag * wlenReal;

                wReal = nextReal;
                wImag = nextImag;
            }
        }
    }
}

// Perform 2D FFT on one sheet
void fft2D(double **real, double **imag, int n)
{
    int i, j;

    double *tempReal = malloc(n * sizeof(double));
    double *tempImag = malloc(n * sizeof(double));

    if (tempReal == NULL || tempImag == NULL)
    {
        printf("Temporary memory allocation failed.\n");
        free(tempReal);
        free(tempImag);
        exit(EXIT_FAILURE);
    }

    // First perform FFT on all rows
    for (i = 0; i < n; i++)
    {
        fft1D(real[i], imag[i], n);
    }

    // Then perform FFT on all columns
    for (j = 0; j < n; j++)
    {
        // Copy column into temporary arrays
        for (i = 0; i < n; i++)
        {
            tempReal[i] = real[i][j];
            tempImag[i] = imag[i][j];
        }

        // Apply FFT to the column
        fft1D(tempReal, tempImag, n);

        // Copy results back into the matrix
        for (i = 0; i < n; i++)
        {
            real[i][j] = tempReal[i];
            imag[i][j] = tempImag[i];
        }
    }

    free(tempReal);
    free(tempImag);
}

int main()
{
    int n, z, y, x;

    FILE *file = fopen("grid3d.txt", "r");

    // Stage 0: Read input file
    if (file == NULL)
    {
        printf("Cannot open grid3d.txt\n");
        return 1;
    }

    if (fscanf(file, "%d", &n) != 1 || n <= 0)
    {
        printf("Invalid dimension in input file.\n");
        fclose(file);
        return 1;
    }

    printf("Dimension N = %d\n", n);

    // Stage 1: Allocate memory
    double *realData = NULL;
    double *imagData = NULL;

    double **realRows = NULL;
    double **imagRows = NULL;

    double ***realGrid =
        allocate3D(n, &realData, &realRows);

    double ***imagGrid =
        allocate3D(n, &imagData, &imagRows);

    if (realGrid == NULL || imagGrid == NULL)
    {
        printf("Memory allocation failed.\n");

        free3D(realGrid, realRows, realData);
        free3D(imagGrid, imagRows, imagData);

        fclose(file);
        return 1;
    }

    printf("Memory allocation completed.\n");

    // Read real values and initialize imaginary values
    for (z = 0; z < n; z++)
    {
        for (y = 0; y < n; y++)
        {
            for (x = 0; x < n; x++)
            {
                if (fscanf(file, "%lf", &realGrid[z][y][x]) != 1)
                {
                    printf("Not enough input values in file.\n");

                    fclose(file);
                    free3D(realGrid, realRows, realData);
                    free3D(imagGrid, imagRows, imagData);

                    return 1;
                }

                imagGrid[z][y][x] = 0.0;
            }
        }
    }

    fclose(file);

    // Stage 2: Check dimension
    if (!isPowerOfTwo(n))
    {
        printf("Invalid N. It must be a power of 2.\n");

        free3D(realGrid, realRows, realData);
        free3D(imagGrid, imagRows, imagData);

        return 1;
    }

    printf("Dimension check passed.\n");

    // Stage 3: Perform 2D FFT on every Z sheet
    for (z = 0; z < n; z++)
    {
        fft2D(realGrid[z], imagGrid[z], n);
    }

    printf("2D FFT completed on all sheets.\n");

    // Stage 4: Transpose data to align Z dimension
    int total = n * n * n;

    double *transReal = malloc((size_t)total * sizeof(double));
    double *transImag = malloc((size_t)total * sizeof(double));

    if (transReal == NULL || transImag == NULL)
    {
        printf("Transpose memory allocation failed.\n");

        free(transReal);
        free(transImag);
        free3D(realGrid, realRows, realData);
        free3D(imagGrid, imagRows, imagData);

        return 1;
    }

    for (x = 0; x < n; x++)
    {
        for (y = 0; y < n; y++)
        {
            for (z = 0; z < n; z++)
            {
                int oldIndex = (z * n + y) * n + x;
                int newIndex = (x * n + y) * n + z;

                transReal[newIndex] = realData[oldIndex];
                transImag[newIndex] = imagData[oldIndex];
            }
        }
    }

    printf("3D matrix transpose completed.\n");

    // Stage 5: Perform FFT along Z dimension
    for (x = 0; x < n; x++)
    {
        for (y = 0; y < n; y++)
        {
            int index = (x * n + y) * n;

            fft1D(&transReal[index], &transImag[index], n);
        }
    }

    printf("Final Z-direction FFT completed.\n");

    // Stage 6: Extract magnitude of slice Z = 0
    printf("\nFinal 2D Magnitude Slice (Z = 0):\n\n");

    for (y = 0; y < n; y++)
    {
        for (x = 0; x < n; x++)
        {
            int index = (x * n + y) * n;

            double r = transReal[index];
            double im = transImag[index];

            double magnitude = sqrt(r * r + im * im);

            printf("%10.3f ", magnitude);
        }

        printf("\n");
    }

    // Stage 7: Free memory
    free(transReal);
    free(transImag);

    free3D(realGrid, realRows, realData);
    free3D(imagGrid, imagRows, imagData);

    printf("\nAll allocated memory has been released.\n");

    return 0;
}