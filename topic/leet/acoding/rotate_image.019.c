/** rotate an image, in-place, without memory allocation
 */

#include <stdio.h>
#include <stdlib.h>

/** solution */
static inline void swap4(int *a, int *b, int *c, int *d) {
    int t = *d;
    *d = *c;
    *c = *b;
    *b = *a;
    *a = t;
}

void rotate(int** m, int rows, int* cols){
    int i, j, ii, jj;
    for (i = 0, ii = rows - 1; i != rows / 2; i++, ii--) {
        for (j = i, jj = rows - i - 1; j != cols[i] - i - 1; j++, jj--) {
            swap4(&m[i][j], &m[j][ii], &m[ii][jj], &m[jj][i]);
        }
    }
}
/** solution end */

void show(int **m, int rows, int *cols) {
    printf("m=%p %d*%d\n", m, rows, cols[0]);
    int i, j;
    for (i = 0; i != rows; i++) {
        for (j = 0; j != cols[i]; j++) {
            printf("%2d ", m[i][j]);
        }
        printf("\n");
    }
}

int unit4() {
    int a[4][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}};
    int *m[4] = {a[0], a[1], a[2], a[3]};
    int cols[] = {4, 4, 4, 4};
    int rows = 4;
    show(m, rows, cols);
    rotate(m, rows, cols);
    show(m, rows, cols);
    return 0;
}
int unit3() {
    int a[3][3] = {{1,2,3}, {4,5,6}, {7, 8, 9}};
    int *m[3] = {a[0], a[1], a[2]};
    int cols[] = {3, 3, 3, 3};
    int rows = 3;
    show(m, rows, cols);
    rotate(m, rows, cols);
    show(m, rows, cols);
    return 0;
}
int unit1() {
    int a[][1] = {{1}};
    int *m[] = {a[0]};
    int cols[] = {1};
    int rows = 1;
    show(m, rows, cols); rotate(m, rows, cols); show(m, rows, cols);
    return 0;
}


int main() {
    unit1();
    unit3();
    unit4();
    return 0;
}
