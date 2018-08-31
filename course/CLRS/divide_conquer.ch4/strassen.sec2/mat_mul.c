#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>


typedef struct {
    int *p;
    int x, y, row, col, stride;
} Mat;

#define CHECK_RANGE(A, i, j)     \
    assert(i >= 0 && i < A.row); \
    assert(j >= 0 && j < A.col);

#define CHECK_MAT_MUL(A, B, C) \
    assert(C.row == A.row);    \
    assert(C.col == B.col);    \
    assert(A.col == B.row)

void mat_init(Mat *A, int *p, int x, int y, int row, int col, int stride) {
    A->p = p;
    A->x = x;
    A->y = y;
    A->row = row;
    A->col = col;
    A->stride = stride;
}

void mat_init_buf(Mat *A, int row, int col) {
    int *p = malloc(sizeof(int) * row * col);
    bzero(p, sizeof(int) * row * col);
    mat_init(A, p, 0, 0, row, col, col);
}

void mat_deinit_buf(Mat *A) {
    free(A->p);
    A->p = NULL;
}

#define ADDRESS(A, i, j) (A.p + A.stride * (A.y + i) + (A.x + j))

int get(Mat A, int i, int j) {
    /* printf("xxx i=%d j=%d A.y=%d A.row=%d\n", i, j, A.y, A.row); */
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
            printf("%d%s", get(A, i, j), j != A.col - 1 ? "," : "");
        }
        printf("]%s\n", i != A.row - 1 ? "," : "]");
    }
}

// A{m,n} * B[r, s] = R[m, s]
void mat_mul(Mat A, Mat B, Mat C) {
    CHECK_MAT_MUL(A, B, C);
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

Mat mat_add_with_scaler(Mat A, Mat B, int k, Mat C) {
    assert(A.row == B.row && A.row == C.row);
    assert(A.col == B.col && A.col == C.col);
    assert(k != 0);
    int i, j;
    for (i = 0; i != A.row; i++) {
        for (j = 0; j != A.col; j++) {
            set(C, i, j, get(A, i, j) + k * get(B, i, j));
        }
    }
    return C;
}

Mat mat_add(Mat A, Mat B, Mat C) {
    return mat_add_with_scaler(A, B, 1, C);
}

Mat mat_sub(Mat A, Mat B, Mat C) {
    return mat_add_with_scaler(A, B, -1, C);
}

void mat_halve(Mat A, Mat *A11, Mat *A12, Mat *A21, Mat *A22) {
    int half = A.row / 2;
    mat_init(A11, A.p, 0, 0, half, half, A.row);
    mat_init(A12, A.p, half, 0, half, half, A.row);
    mat_init(A21, A.p, 0, half, half, half, A.row);
    mat_init(A22, A.p, half, half, half, half, A.row);
}

#define M_NUM   8 // we need one extra buffer to store temp matrix
#define CHECK_SQUARE(A) assert(A.row == A.col)
void mat_mul_strassen_square_even(Mat A, Mat B, Mat C, void *f) {
    CHECK_MAT_MUL(A, B, C);
    CHECK_SQUARE(A);
    CHECK_SQUARE(B);
    CHECK_SQUARE(C);
    assert(A.row % 2 == 0);
    int half = A.row / 2;
    void (*fn)(Mat, Mat, Mat, void *) = f;
    Mat A11, A12, A21, A22;
    mat_halve(A, &A11, &A12, &A21, &A22);
    Mat B11, B12, B21, B22;
    mat_halve(B, &B11, &B12, &B21, &B22);
    Mat C11, C12, C21, C22;
    mat_halve(C, &C11, &C12, &C21, &C22);
    Mat M[M_NUM];
    int i;
    for (i = 0; i != M_NUM; i++) {
        mat_init_buf(&M[i], half, half);
    }

    // only need calling fn 7 times, but not 8 times
    fn(mat_sub(A12, A22, M[0]), mat_add(B21, B22, M[1]), M[7], fn);
    fn(mat_sub(A21, A11, M[0]), mat_add(B11, B12, M[1]), M[6], fn);
    fn(mat_add(A11, A22, M[0]), mat_add(B11, B22, M[2]), M[1], fn);
    fn(mat_add(A11, A12, M[0]), B22, M[5], fn);
    fn(A22, mat_sub(B21, B11, M[0]), M[4], fn);
    fn(A11, mat_sub(B12, B22, M[0]), M[3], fn);
    fn(mat_add(A21, A22, M[0]), B11, M[2], fn);

    mat_sub(mat_add(mat_add(M[7], M[4], C11), M[1], C11), M[5], C11);
    mat_add(M[3], M[5], C12);
    mat_add(M[2], M[4], C21);
    mat_sub(mat_add(mat_add(M[1], M[3], C22), M[6], C22), M[2], C22);

    for (i = 0; i != M_NUM; i++) {
        mat_deinit_buf(&M[i]);
    }
}

void mat_mul_strassen_square(Mat A, Mat B, Mat C, void *f) {
    assert(C.p != A.p && C.p != B.p);
    if (A.row == 1 && A.col == 1) {
        set(C, 0, 0, get(A, 0, 0) * get(B, 0, 0));
        return;
    }
    mat_mul_strassen_square_even(A, B, C, f);
}

void mat_mul_strassen(Mat A, Mat B, Mat C) {
    mat_mul_strassen_square(A, B, C, mat_mul_strassen_square);
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

void basic_2x2(void (*f)(Mat, Mat, Mat)) {
    int pa[] = {1, 3, 7, 5};
    int pb[] = {6, 8, 4, 2};
    int pc[4] = {0};
    int pe[4] = {18, 14, 62, 66};
    Mat A, B, C, E;
    mat_init(&A, pa, 0, 0, 2, 2, 2);
    mat_init(&B, pb, 0, 0, 2, 2, 2);
    mat_init(&C, pc, 0, 0, 2, 2, 2);
    mat_init(&E, pe, 0, 0, 2, 2, 2);
    f(A, B, C);
    mat_show(A);
    mat_show(B);
    mat_show(C);
    assert(mat_eq(C, E) == true);
}

void basic_2x3(void (*f)(Mat, Mat, Mat)) {
    int pa[] = {1, 3, 7, 5, 9, 10};
    int pb[] = {6, 8, 4, 2, 0, 11};
    int pc[4] = {0};
    int pe1[4] = {18, 91, 66, 168};
    int pd[9] = {0};
    int pe2[9] = {46, 90, 122, 14, 30, 48, 55, 99, 110};
    Mat A, B, C, D, E1, E2;
    mat_init(&A, pa, 0, 0, 2, 3, 3);
    mat_init(&B, pb, 0, 0, 3, 2, 2);
    mat_init(&C, pc, 0, 0, 2, 2, 2);
    mat_init(&E1, pe1, 0, 0, 2, 2, 2);
    mat_init(&D, pd, 0, 0, 3, 3, 3);
    mat_init(&E2, pe2, 0, 0, 3, 3, 3);
    mat_show(A);
    mat_show(B);
    f(A, B, C);
    f(B, A, D);
    mat_show(C);
    mat_show(D);
    assert(mat_eq(C, E1) == true);
    assert(mat_eq(D, E2) == true);
}

int main() {
    /* basic_2x2(mat_mul); */
    /* basic_2x3(mat_mul); */
    basic_2x2(mat_mul_strassen);
    return 0;
}
