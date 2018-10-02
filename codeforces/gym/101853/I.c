#include <stdio.h>
#include <inttypes.h>

int main() {
    int T;
    scanf("%d", &T);
    for(; T != 0; T--) {
        int a, b;
        uint64_t d; // d <= 10**9, must consider overflow issue
        scanf("%d%d%I64lld", &a, &b, &d);
        printf("%f\n", d * d / 2.);
    }
}
