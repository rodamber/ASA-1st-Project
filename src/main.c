#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void
print_array_h(const int *a, size_t nmemb)
{
    for (size_t i = 0; i < nmemb - 1; i++) {
        printf("+---");
    }
    printf("+---+\n");

    for (size_t i = 0; i < nmemb; i ++) {
        printf("| %d ", a[i]);
    }
    printf("|\n");

    for (size_t i = 0; i < nmemb - 1; i++) {
        printf("+---");
    }
    printf("+---+\n");
}

void
print_array_v(const int *a, size_t nmemb)
{
    printf("+---+\n");
    for (size_t i = 0; i < nmemb; i++) {
        printf("| %d |\n", a[i]);
        printf("+---+\n");
    }
}

void
print_int(const void * p)
{
    printf("%d", *((int*) p));
}

int
random_mm (const int min, const int max)
{
    return min + rand() % (max - min + 1);
}

void
csort(int *a, const int min, const int max, const int l, const int r)
{
    const int csize = max - min + 1;
    const int asize = r - l + 1;

    int *count = calloc(csize, sizeof (int));
    int *buf   = malloc(asize * sizeof (int));

    for (int i = l; i <= r; i++) {
        count[a[i] - min]++;
    }

    for (int i = 1; i < csize; i++) {
        count[i] += count[i - 1];
    }

    for (int i = r; i >= l; i--) {
        buf[count[a[i] - min] - 1] = a[i];
        --count[a[i] - min];
    }
    free(count);

    for (int i = r; i >= l; i--) {
        a[i] = buf[i - l];
    }
    free(buf);
}

#define SIZE 21
#define MIN  0
#define MAX  5

int
main(void)
{
    int a[SIZE];

    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        a[i] = random_mm(MIN, MAX);
    }

    char *padding = calloc(1 + SIZE / 2, sizeof (char));

    int i = 0;
    while (padding[i++] = ' ')
        ;

    printf("%s=============", padding);
    printf("%sCounting Sort", padding);
    printf("%s=============", padding);

    csort(a, MIN, MAX, 0, SIZE-1);

    print_array_h(a, SIZE);
    //print_array_v(a, SIZE);

    return 0;
}
