#include "leet.h"

unsigned long long numTrees(unsigned long long n) {
    return catalan(n);
}

int main() {
    int i;
    for (i = 0; i != 25; i++) {
        printf("%d: %llu\n", i, numTrees(i));
    }
    return 0;
}
