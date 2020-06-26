/** check incompleted sudoku have solution or not */
/** 1: easy */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define B 3
#define W (B * B)


#define MEET(cond) { if (!(cond)) { return false; }}

bool isBlock(int *a, int i0, int i1, int j0, int j1) {
    // MEET((i1 - i0) * (j1 - j0) == W);
    assert((i1 - i0) * (j1 - j0) == W);
    int c[W];
    memset(c, 0, sizeof(int) * W);
    int i, j;
    for (i = i0; i != i1; i++) {
        for (j = j0; j != j1; j++) {
            int x = a[i * W + j];
            if (x == 0) { // skip empty block
                continue;
            }
            // if (c[x-1] != 0) {
            //     printf("x=%d c=%d at block=%d,%dx%d,%d\n", x, c[x-1], i0, i1, j0, j1);
            // }
            MEET(c[x - 1] == 0); // only once
            c[x - 1] += 1;
        }
    }
    return true;
}

bool isSudoku(int *a) {
    int i, j;
    for (i = 0; i != W; i++) {
        MEET(isBlock(a, i, i + 1, 0, 9));
    }
    for (i = 0; i != W; i++) {
        MEET(isBlock(a, 0, 9, i, i + 1));
    }
    for (i = 0; i != W; i += 3) {
        for (j = 0; j != W; j += 3) {
            MEET(isBlock(a, i, i + 3, j, j + 3));
        }
    }
    return true;
}

void show_sudoku(int *a) {
    int i, j;
    for (i = 0; i != W; i++) {
        for (j = 0; j != W; j++) {
            printf("%d ", a[i * W + j]);
            if ((j + 1) % B == 0) {
                printf(" ");
            }
        }
        printf("\n");
        if ((i + 1) % B == 0) {
            printf("\n");
        }
    }
}

int main() {
    int t, m[W * W], i;
    for (scanf("%d", &t); t != 0; t--) {
        for (i = 0; i != W * W; i++) {
            scanf("%d", &m[i]);
        }
        // show_sudoku(m);
        printf("%d\n", isSudoku(m));
    }
    return 0;
}
