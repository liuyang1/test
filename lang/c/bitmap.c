#include <stdio.h>
#include <string.h>

void showArr(unsigned int *a, int len) {
    int i;
    for (i = 0; i != len; i++) {
        printf("%08x ", a[i]);
    }
    printf("\n");
}

void assign(unsigned int *a, int idx) {
    a[(idx >> 5) & 3] |= 1 << idx;
}

void unassign(unsigned int *a, int idx) {
    if (!(a[(idx >> 5) & 3] & (1 << idx))) {
        showArr(a, 4);
        printf("except arr on idx=%d\n", idx);
        return;
    }
    a[(idx >> 5) & 3] &= ~(1 << idx);
}

unsigned int sumBit(unsigned int v) {
    unsigned int c;
    for (c = 0; v; c++) {
        v &= v - 1;
    }
    return c;
}

unsigned int sumBitArr(unsigned int *a, int len) {
    unsigned int c;
    int i;
    for (i = 0, c = 0; i != len; i++) {
        c += sumBit(a[i]);
    }
    return c;
}
int main() {
    unsigned int a[4] = {0xffffffff};
    showArr(a, 4);
    memset(a, 0, sizeof(a));
    showArr(a, 4);
    assign(a, 0);
    assign(a, 1);
    assign(a, 31);
    assign(a, 32);
    assign(a, 63);
    assign(a, 64);
    // assign(a, 127);
    // assign(a, 100);
    // unassign(a, 100);
    // unassign(a, 127);
    // unassign(a, 32);
    showArr(a, 4);
    unsigned int c = sumBitArr(a, 4);
    printf("bit: %d\n", c);
    printf("len: %lu\n", sizeof(a));
    printf("len: %lu\n", sizeof(a) / sizeof(a[0]));

    int idx = 31;
    printf("%08x\n", 1 << idx);
    // circular shifting
    idx = 32;
    printf("%08x\n", 1 << idx);
    idx = 63;
    printf("%08x\n", 1 << idx);
    idx = 64;
    printf("%08x\n", 1 << idx);

    // WARNING, and show result == 0x0
    printf("%08x\n", 1 << 63);


    return 0;
}
