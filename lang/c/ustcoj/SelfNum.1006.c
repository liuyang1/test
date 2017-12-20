#include <stdio.h>
#define NUM 10000

int gen(int a) {
    int n = a;
    while (n != 0) {
        a += n % 10;
        n /= 10;
    }
    return a;
}

int main() {
    int a[NUM] = {0};
    int i;
    for (i = 1; i != NUM; i++) {
        int n = i;
        while (1) {
            n = gen(n);
            if (a[n] == 1 || n >= NUM) {
                break;
            }
            a[n] = 1;
        }
    }
    for (i = 1; i != NUM; i++) {
        if (a[i] != 1) {
            printf("%d\n", i);
        }
    }
    return 0;
}
