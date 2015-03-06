#ifndef __UTILS_H__
#define __UTILS_H__

void
print_array_h(const int *a, size_t nmemb)
{
    size_t i;

    for (i = 0; i < nmemb - 1; i++) {
        printf("+---");
    }
    printf("+---+\n");

    for (i = 0; i < nmemb; i++) {
        printf("| %d ", a[i]);
    }
    printf("|\n");

    for (i = 0; i < nmemb - 1; i++) {
        printf("+---");
    }
    printf("+---+\n");
}

int
random_mm (const int min, const int max)
{
    return min + rand() % (max - min + 1);
}

#endif
