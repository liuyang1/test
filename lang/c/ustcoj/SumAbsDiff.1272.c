#include <stdio.h>
#include <stdlib.h>

unsigned long long sumAbsDiff(int *a, size_t la) {
    // a[i] <= 200, so we could optimize it.
    size_t i, j;
    unsigned long long sum;
#define MAXLEN      300
    int *b = malloc(sizeof(int) * MAXLEN);
    for (i = 0; i != MAXLEN; i++) {
        b[i] = 0;
    }
    for (i = 0; i != la; i++) {
        b[a[i]]++;
    }
    for (i = 0, sum = 0; i != MAXLEN; i++) {
        for (j = i; j != MAXLEN; j++) {
            sum += abs(i - j) * b[i] * b[j];
        }
    }
    free(b);
    return sum;
}

int main() {
    int casenum;
    scanf("%d", &casenum);
    while (casenum--) {
        size_t la, i;
        scanf("%u", &la);
        int *a = malloc(sizeof(int) * la);
        for (i = 0; i != la; i++) {
            scanf("%d", &a[i]);
        }
        printf("%llu\n", sumAbsDiff(a, la));
        free(a);
    }
    return 0;
}
