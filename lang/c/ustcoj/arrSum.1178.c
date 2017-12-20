#include <stdio.h>

double arrSum(unsigned long long n) {
    unsigned long long i, a = 1, b = 2, t;
    double s = 0;
    for (i = 0; i != n; i++, t = b, b = a + b, a = t) {
        s += b / (a + 0.0);
    }
    return s;
}

int main() {
    unsigned long long n;
    while (scanf("%llu", &n) == 1) {
        printf("%.6f\n", arrSum(n));
    }
    return 0;
}
