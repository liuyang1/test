#include <stdio.h>
typedef union {
    double f;
    long long int x;
} T1;

typedef union {
    float f;
    int x;
} T2;

int main() {
    T1 t;
    t.f = 0.15625;
    printf("%llx %lf %zu\n", t.x, t.f, sizeof(t));

    T2 t2;
    t2.f = 0.15625;
    printf("%x %f %zu\n", t2.x, t2.f, sizeof(t2));

    return 0;
}
