#include <stdio.h>

int main() {
    int t;
    scanf("%d" ,&t);
    for (; t != 0; t--) {
        int n, m, k;
        scanf("%d%d%d", &n, &m, &k);
        printf("%f\n", floor_height(n, m, k));
    }
    return 0;
}
