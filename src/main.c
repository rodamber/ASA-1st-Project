#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void
print_array(int *v, int size, char c)
{
    for (int i = 0; i < size; i++) {
        printf("%c[%d]=%d\n", c, i, v[i]);
    }
}

int
random_mm (const int min, const int max)
{
    return min + rand() % (max - min + 1);
}

void
csort(int v[], const int min, const int max, const int l, const int r)
{
    const int csize = max - min + 1;
    const int vsize = r - l + 1;

    int *count = calloc(csize, sizeof (int));
    int *buf   = malloc(vsize * sizeof (int));

    for (int i = l; i <= r; i++) {
        count[v[i] - min]++;
    }

    for (int i = 1; i < csize; i++) {
        count[i] += count[i - 1];
    }

    for (int i = r; i >= l; i--) {
        buf[count[v[i] - min] - 1] = v[i];
        --count[v[i] - min];
    }

    for (int i = r; i >= l; i--) {
        v[i] = buf[i - l];
    }

    free(count);
    free(buf);
}

#define SIZE  8
#define MIN 0
#define MAX 5

int
main(void)
{
    int A[SIZE];

    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        A[i] = random_mm(MIN, MAX);
    }

    puts("=============");
    puts("Counting Sort");
    puts("=============");
    csort(A, MIN, MAX, 0, SIZE-1);
    print_array(A, SIZE, 'A');

    return 0;
}
