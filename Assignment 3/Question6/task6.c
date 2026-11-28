#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;

typedef struct {
    ull sum;
    int a;
    int b;
} CubePair;

/* Compare pairs by sum first, then by a and b */
int compare_pairs(const void *p1, const void *p2) {
    const CubePair *x = (const CubePair *)p1;
    const CubePair *y = (const CubePair *)p2;

    if (x->sum < y->sum) return -1;
    if (x->sum > y->sum) return 1;
    if (x->a != y->a) return (x->a < y->a) ? -1 : 1;
    if (x->b != y->b) return (x->b < y->b) ? -1 : 1;
    return 0;
}

/* Generate all pairs (a^3 + b^3, a, b) for 1 <= a <= b <= limit */
CubePair *generate_pairs(int limit, size_t *out_count) {
    size_t count = (size_t)limit * (size_t)(limit + 1) / 2;
    CubePair *pairs = malloc(count * sizeof(CubePair));
    if (!pairs) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    size_t idx = 0;
    for (int a = 1; a <= limit; a++) {
        ull a3 = (ull)a * (ull)a * (ull)a;
        for (int b = a; b <= limit; b++) {
            ull b3 = (ull)b * (ull)b * (ull)b;
            pairs[idx].sum = a3 + b3;
            pairs[idx].a = a;
            pairs[idx].b = b;
            idx++;
        }
    }

    *out_count = count;
    return pairs;
}

/* Print a taxicab number with its rank and two distinct cube pairs */
void print_taxicab(int rank, ull sum, CubePair p1, CubePair p2) {
    printf("%d. %llu = %d^3 + %d^3 = %d^3 + %d^3\n",
           rank, sum, p1.a, p1.b, p2.a, p2.b);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    char *endptr = NULL;
    long n_val = strtol(argv[1], &endptr, 10);
    if (*argv[1] == '\0' || *endptr != '\0' || n_val < 1) {
        printf("Error: N must be a positive integer (>= 1).\n");
        return 1;
    }
    int n = (int)n_val;

    int found = 0;
    int limit = 2;
    ull last_printed_sum = 0;
    int has_printed = 0;

    while (found < n) {
        size_t pair_count = 0;
        CubePair *pairs = generate_pairs(limit, &pair_count);
        qsort(pairs, pair_count, sizeof(CubePair), compare_pairs);

        size_t i = 0;
        while (i < pair_count) {
            size_t j = i + 1;
            /* Find all pairs with the same sum */
            while (j < pair_count && pairs[j].sum == pairs[i].sum) {
                j++;
            }

            /* Check if we have at least 2 distinct pairs for this sum */
            size_t group_len = j - i;
            if (group_len >= 2) {
                ull current_sum = pairs[i].sum;
                if (!has_printed || current_sum != last_printed_sum) {
                    found++;
                    print_taxicab(found, current_sum, pairs[i], pairs[i + 1]);
                    last_printed_sum = current_sum;
                    has_printed = 1;

                    if (found == n) {
                        free(pairs);
                        return 0;
                    }
                }
            }
            i = j;
        }

        free(pairs);
        limit *= 2; /* Expand search space dynamically if fewer than N found */
    }

    return 0;
}