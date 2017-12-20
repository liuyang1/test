#include <stdio.h>

int fib(int n) {
    if (n <= 2) {
        return n;
    }
    int a = 2, b = 1, c = 0;
    for (n -= 2; n != 0; n--) {
        int t = a;
        a = a + b + c;
        c = b;
        b = t;
    }
    return a;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        printf("The New Fibonacci number for %d is %d\n", n, fib(n));
    }
    return 0;
}
