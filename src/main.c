#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

void
csort(int *a, const int min, const int max, const int l, const int r)
{
    const int csize = max - min + 1;
    const int asize = r - l + 1;

    int *count = calloc(csize, sizeof (int));
    int *buf   = malloc(asize * sizeof (int));

    int i;

    for (i = l; i <= r; i++) {
        count[a[i] - min]++;
    }

    for (i = 1; i < csize; i++) {
        count[i] += count[i - 1];
    }

    for (i = r; i >= l; i--) {
        buf[count[a[i] - min] - 1] = a[i];
        --count[a[i] - min];
    }
    free(count);

    for (i = r; i >= l; i--) {
        a[i] = buf[i - l];
    }
    free(buf);
}

#define SIZE 20
#define MIN  0
#define MAX  5

int
main(void)
{
    int a[SIZE];
    int i;

    const int pad_size = 4 * (SIZE / 2 - 2);
    char *padding = calloc(pad_size, sizeof (char));

    srand(time(NULL));
    for (i = 0; i < SIZE; i++) {
        a[i] = random_mm(MIN, MAX);
    }

    for (i = 0; i < pad_size; i++) {
        padding[i] = ' ';
    }

    printf("%s=============\n", padding);
    printf("%sCounting Sort\n", padding);
    printf("%s=============\n", padding);
    csort(a, MIN, MAX, 0, SIZE-1);
    print_array_h(a, SIZE);

    free(padding);
    return 0;
}
