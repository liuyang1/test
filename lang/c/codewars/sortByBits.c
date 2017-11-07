#include <stdio.h>
#include <stdlib.h>

int countBits(int a) {
    int cnt;
    for (cnt = 0; a != 0; a &= a - 1, cnt++) {
    }
    return cnt;
}

int sign(int a) {
    return (a > 0) - (a < 0);
}

int compareByBits(const void *pa, const void *pb) {
    int a = *(int const *)pa;
    int b = *(int const *)pb;
    int s = sign(countBits(a) - countBits(b));
    if (s == 0) {
        s = sign(a - b);
    }
    printf("compare(%d %d,  %d %d) = %d\n", a, countBits(a), b, countBits(b), s);
    return s;
}

void sort_by_bit(int *arr, int arrLength) {
    qsort(arr, arrLength, sizeof(int), compareByBits);
}

// test code
int unit(int *arr, int num) {
    sort_by_bit(arr, num);
    for (int i = 0; i != num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}

int test() {
    int a[] = {3, 8, 3, 6, 5, 7, 9, 1};
    unit(a, sizeof(a) / sizeof(a[0]));
    return 0;
}

int main() {
    test();
    return 0;
}
