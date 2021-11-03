#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len, const char *label) {
    printf("%20s", label);
    size_t i;
    for (i = 0; i < len; i++) {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) & x, sizeof(int), __func__);
}

void show_float(float x) {
    show_bytes((byte_pointer) & x, sizeof(float), __func__);
}


void show_pointer(void *x) {
    show_bytes((byte_pointer) & x, sizeof(void *), __func__);
}

void show_short(short x) {
    show_bytes((byte_pointer) & x, sizeof(short), __func__);
}

void show_long(long x) {
    show_bytes((byte_pointer) & x, sizeof(long), __func__);
}

void show_double(double x) {
    show_bytes((byte_pointer) & x, sizeof(double), __func__);
}

int unit(int x) {
    show_short(x);
    show_int(x);
    show_long(x);

    show_float(x);
    show_double(x);

    show_pointer(&x);
    return 0;
}

int main() {
    // P2.56 different sample values
    unit(42);
    unit(6666);
    return 0;
}
