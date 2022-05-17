#include <stdio.h>

int main() {
    int t;
    scanf("%d", &t);
    for (; t != 0; t--) {
        int n, m, k;
        scanf("%d%d%d", &n, &m, &k);
        double h = (k + 0.) / m * (n - 1);
        printf("%f\n", h);
    }
    return 0;
}
