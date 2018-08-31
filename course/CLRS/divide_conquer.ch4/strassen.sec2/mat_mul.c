#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct {
    int *p;
    int x, y, row, col, stride;
} Mat;

#define CHECK_RANGE(A, i, j)             \
    assert(i >= A.y && i < A.y + A.row); \
    assert(j >= A.x && j < A.x + A.col);

void init(Mat *A, int *p, int x, int y, int row, int col, int stride) {
    A->p = p;
    A->x = x;
    A->y = y;
    A->row = row;
    A->col = col;
    A->stride = stride;
}

#define ADDRESS(A, i, j) (A.p + A.stride * (A.y + i) + (A.x + j))

int get(Mat A, int i, int j) {
    CHECK_RANGE(A, i, j);
    return *ADDRESS(A, i, j);
}

void set(Mat A, int i, int j, int v) {
    CHECK_RANGE(A, i, j);
    *ADDRESS(A, i, j) = v;
}

void mat_show(Mat A) {
    int i, j;
    printf("mat %dx%d\n", A.row, A.col);
    for (i = 0; i != A.row; i++) {
        printf("%s[", i == 0 ? "[" : " ");
        for (j = 0; j != A.col; j++) {
            printf("%d%s", get(A, i, j), j != A.col - 1? "," : "");
        }
        printf("]%s\n", i != A.row - 1 ? "," : "]");
    }
}

// A{m,n} * B[r, s] = R[m, s]
void mat_mul(Mat A, Mat B, Mat C) {
    assert(C.row = A.row);
    assert(C.col = B.col);
    assert(A.col = B.row);
    int i, j, k, s;
    for (i = 0; i != C.row; i++) {
        for (j = 0; j != C.col; j++) {
            s = 0;
            for (k = 0; k != A.col; k++) {
                s += get(A, i, k) * get(B, k, j);
            }
            set(C, i, j, s);
        }
    }
}

void mat_mul_strassen(Mat A, Mat B, Mat C) {

}

bool mat_eq(Mat A, Mat B) {
    if (A.row == B.row && A.col == B.col) {
        int i, j;
        for (i = 0; i != A.row; i++) {
            for (j = 0; j != A.col; j++) {
                if (get(A, i, j) != get(B, i, j)) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

void basic_2x2() {
    int pa[] = {1, 3, 7, 5};
    int pb[] = {6, 8, 4, 2};
    int pc[4] = {0};
    int pe[4] = {18, 14, 62, 66};
    Mat A, B, C, E;
    init(&A, pa, 0, 0, 2, 2, 2);
    init(&B, pb, 0, 0, 2, 2, 2);
    init(&C, pc, 0, 0, 2, 2, 2);
    init(&E, pe, 0, 0, 2, 2, 2);
    mat_mul(A, B, C);
    mat_show(A);
    mat_show(B);
    mat_show(C);
    assert(mat_eq(C, E) == true);
}

void basic_2x3() {
    int pa[] = {1, 3, 7, 5, 9, 10};
    int pb[] = {6, 8, 4, 2, 0, 11};
    int pc[4] = {0};
    int pe1[4] = {18, 91, 66, 168};
    int pd[9] = {0};
    int pe2[9] = {46, 90, 122, 14, 30, 48, 55, 99, 110};
    Mat A, B, C, D, E1, E2;
    init(&A, pa, 0, 0, 2, 3, 3);
    init(&B, pb, 0, 0, 3, 2, 2);
    init(&C, pc, 0, 0, 2, 2, 2);
    init(&E1, pe1, 0, 0, 2, 2, 2);
    init(&D, pd, 0, 0, 3, 3, 3);
    init(&E2, pe2, 0, 0, 3, 3, 3);
    mat_show(A);
    mat_show(B);
    mat_mul(A, B, C);
    mat_mul(B, A, D);
    mat_show(C);
    mat_show(D);
    assert(mat_eq(C, E1) == true);
    assert(mat_eq(D, E2) == true);
}

int main() {
    basic_2x2();
    basic_2x3();
    return 0;
}
