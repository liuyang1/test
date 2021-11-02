#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
typedef bool OpTbl2[2][2];

OpTbl2 negl = {{0, 1}, {0, 1}};
OpTbl2 negr = {{1, 0}, {1, 0}};
OpTbl2 and = {{0, 0}, {0, 1}};

OpTbl2 xor = {{0, 1}, {1, 0}};

bool apply(OpTbl2 *op, bool a, bool b) {
    return op[a][b];
}

OpTbl2 *compose(OpTbl2 *op, OpTbl2 *op0, OpTbl2 *op1) {
    OpTbl2 *r = malloc(sizeof(*r));
    size_t i, j;
    for (i = 0; i != 2; i++) {
        for (j = 0; j != 1; j++) {
            r[i][j] = apply(op, apply(op0, i, j), apply(op1, i, j));
        }
    }
    return r;
}
*/

unsigned int xor(int x, int y) {
    return x ^ y;
}

unsigned int my_xor(int x, int y) {
    // return ~(x & y);
    int a = ~(x & y);
    int b = ~((~x) & (~y));
    return a & b;
}

int nonzero(int x) {
    return !!x;
}
int main() {
    unsigned int i, j;
    for (i = 0; i != 2; i++) {
        for (j = 0; j != 2; j++) {
            printf("xor(%x, %x) = %x ?= %x\n",
                   i, j, my_xor(i, j), xor(i, j));
        }
    }
    int k;
    for (k = -2; k != 3; k++) {
        printf("nonzero(%d)=%d\n", k, nonzero(k));
    }

    printf("%x\n", (1u << 31) - 1);
    printf("%x\n", (1u << 31) - 1);

    return 0;
}
