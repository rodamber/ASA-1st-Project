#include <stdio.h>
#include <stdlib.h>

#define SIZE 20
#define RANGE 100

void print_vector(int* v, int size, char c) {
    for (int i = 0; i < size; i++) {
        printf("%c[%d]=%d\n", c, i, v[i]);
    }
}

void counting_sort(int* A, int* B, int size, int range) {
    int* C = malloc(sizeof(int) * (range + 1));

    for (int i = 0; i < range + 1; i++) {
        C[i] = 0;
    }

    for (int j = 0; j < size; j++) {
        C[ A[j] ]++;
    }
    // C[i] now contains the number of elements equal to i.

    for (int i = 1; i < range + 1; i++) {
        C[i] += C[i - 1];
    }
    // C[i] now contains the number of elements less than or equal to i.

    for (int j = size - 1; j >= 0; j--) {
        B[ C[ A[j] ] - 1 ] = A[j];
        C[ A[j] ]--;
    }

    free(C);
}

int main(int argc, char** argv) {
    int A[SIZE];
    int B[SIZE];

    for (int i = 0; i < SIZE; i++) {
        A[i] = rand() % RANGE;
    }

    counting_sort(A, B, SIZE, RANGE);
    puts("------------------");
    print_vector(A, SIZE, 'A');
    puts("------------------");
    print_vector(B, SIZE, 'B');

    return 0;
}
