#include <stdio.h>
#include <math.h>
#include <stdbool.h>

bool isSquare(unsigned int n) {
    unsigned int b = 0, e = 65536;
    unsigned int m;
    while (b <= e) {
        m = (b + e) / 2;
        unsigned int m2 = m * m;
        if (m2 == n) {
            return true;
        }
        if (m2 < n) {
            b = m + 1;
        } else {
            e = m - 1;
        }
    }
    return false;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 0) {
            break;
        }
        printf("%s\n", isSquare(n) ? "Yes" : "No");
    }
    return 0;
}
