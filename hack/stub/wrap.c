#include <stdio.h>

int cnt = 0;

int func(int x) {
    cnt++;
    printf("func call times=%d\n", cnt);
    return real_func(x);
}
