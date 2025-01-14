#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {

    printf("Standard malloc:\n\n");

    int *k = (int*) malloc(5 * sizeof(int));
    int *y = (int*) calloc(5, sizeof(int));
    k = (int*) realloc(k, 10 * sizeof(int));
    free(k);
    free(y);

    int *xy = (int*) malloc(5 * sizeof(int));
    printf("Modified malloc:\n");


    printf("(Run injector) Press Enter to continue...");
    getchar();  // Wait for the user to press something
    printf("\nContinuing the program...\n");


    xy = (int*) realloc(xy, 10 * sizeof(int));
    free(xy);

    int *a = (int*) malloc(5 * sizeof(int));
    int *b = (int*) malloc(5 * sizeof(int));
    int *c = (int*) calloc(10, sizeof(int));
    int *d = (int*) malloc(5 * sizeof(int));
    int *e = NULL;

    a = (int*) realloc(a, 10 * sizeof(int));
    a = (int*) realloc(a, 0);

    int *g = (int*) realloc(NULL, 10 * sizeof(int));

    free(b);
    free(g);
    // free(b); // Uncommenting this will cause a double free and program will crash
    // free(e); // Pointer hasn't been allocated yet -> program will crash

    printf("(Run uninjector) Press Enter to continue...");
    getchar();  // Wait for the user to press something
    printf("\nContinuing the program...\n");

    printf("\nAgain standard malloc:\n");
    int *x = (int*) malloc(5 * sizeof(int));
    int *l = (int*) calloc(5, sizeof(int));
    x = (int*) realloc(x, 10 * sizeof(int));
    free(x);
    free(l);

    Sleep(6000); // Added just to see the output

    return 0;
}