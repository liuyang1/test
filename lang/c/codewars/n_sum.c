#include <stdio.h>
int n_sum(int n, ...) {
    int *p = &n;
    int i, s;
    int *a = (int *)(p + 11); // magic number for Linux x86-64 platform
    for (i = s = 0; i != n; a += 2, i++) {
        // printf("%p %d\n", a, *a);
        s += *a;
    }
    return s;

}

int main() {
    int s;
    s = n_sum(0);
    if (s != 0) {
        printf("find error case zero\n");
    }
    s = n_sum(1, 3);
    if (s != 3) {
        printf("find error case 0\n");
    }
    s = n_sum(2, 3, 5);
    if (s != 8) {
        printf("find error case 1\n");
    }
    s = n_sum(3, 3, 5, 7);
    if (s != 15) {
        printf("find error case 2\n");
    }
    s = n_sum(4, 3, 5, 7, 9);
    if (s != 24) {
        printf("find error case 3\n");
    }
    return 0;
}
