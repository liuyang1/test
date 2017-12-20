/** also for 1054
 */
#include <stdio.h>

int fib(int n) {
    if (n <= 1) {
        return n;
    }
    int a = 0, b = 1;
    for (; n != 0; n--) {
        int t = a;
        a = a + b;
        b = t;
    }
    return a;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        printf("The Fibonacci number for %d is %d\n", n, fib(n));
    }
    return 0;
}
