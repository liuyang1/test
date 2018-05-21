#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int sum(int *a, unsigned int size) {
    int s = 0;
    unsigned int i;
    for (i = 0; i != size; i++) {
        s += a[i];
    }
    return s;
}

double entropy(int *a, unsigned int size, int sum) {
    unsigned int i;
    double ret = 0;
    for (i = 0; i != size; i++) {
        if (a[i] == 0) {
            continue;
        }
        double p = a[i] / (sum + 0.0);
        ret += -1 * p * log2(p);
    }
    return ret;
}

#define SIZE (1024*1024)
int main() {
    int arr[SIZE] = {0};
    int n = 0;
    while (true) {
        int v;
        if (scanf("%d", &v) < 0) {
            break;
        }
        arr[n] = v;
        n++;
        if (n == SIZE) {
            fprintf(stderr, "need enlarge buffer size=%d\n", SIZE);
            exit(-1);
        }
    }
    int s = sum(arr, n);
    double e = entropy(arr, n, s);
    printf("sum=%d entropy=%f\n", s, e);
    return 0;
}
