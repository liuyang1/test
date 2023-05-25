#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv) {


    int n = atoi(argv[1]);
    double a[n];
    double a__[2 * n];

    double *c = malloc(sizeof(double)*n);

#define N 1024
    double b[N];
    printf("%d stack=%p a=%p/vla b=%p/static c=%p/heap\n", n, &n, a, b, c);
    int i;
    for (i = 0; i != 5; i++) {
        printf("%.4f\n", a[i]);
    }
    return 0;
}
