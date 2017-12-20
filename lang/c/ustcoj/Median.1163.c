#include <stdio.h>
#include <stdlib.h>

void show(double *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%f ", a[i]);
    }
    printf("\n");
}

int cmp(const void *a, const void *b) {
    const double *x = a, *y = b;
    return (*x > *y) - (*x < *y);
}

double median(double *a, int n) {
    // simple and stupid qsort.
    // It's O(nlogn). If only want median, It's O(n)
    qsort(a, n, sizeof(double), cmp);
    if (n % 2 == 0) {
        return (a[n / 2] + a[n / 2 - 1]) / 2;
    } else {
        return a[n / 2];
    }
}

int main() {
    int casenum, i, j;
    scanf("%d", &casenum);
    for (i = 0; i != casenum; i++) {
        int num;
        scanf("%d", &num);
        double *a = malloc(sizeof(double) * num);
        for (j = 0; j != num; j++) {
            scanf("%lf", &a[j]);
        }
        printf("%.3f\n", median(a, num));
        free(a);
    }
    return 0;
}
