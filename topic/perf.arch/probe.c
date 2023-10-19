#include <stdio.h>
#include <stdlib.h>
int main() {
    __builtin_cpu_init();
#define CHECK(X) if (__builtin_cpu_supports(X)) { printf("%s\n", X); }
    CHECK("cmov");
    CHECK("mmx");
    CHECK("popcnt");
    CHECK("sse");
    CHECK("sse2");
    CHECK("sse3");
    CHECK("ssse3");
    CHECK("sse4.1");
    CHECK("sse4.2");
    CHECK("avx");
    CHECK("avx2");
    return 0;
}
