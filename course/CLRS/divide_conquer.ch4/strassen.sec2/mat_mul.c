#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#include <sys/time.h>
#include <pthread.h>

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

static void mat_init(Mat *A, int *p,
                     int x, int y, int row, int col, int stride) {
    A->p = p;
    A->x = x;
    A->y = y;
    A->row = row;
    A->col = col;
    A->stride = stride;
}

static void mat_init_buf(Mat *A, int row, int col) {
    int *p = malloc(sizeof(int) * row * col);
    bzero(p, sizeof(int) * row * col);
    mat_init(A, p, 0, 0, row, col, col);
}

static void mat_deinit_buf(Mat *A) {
    free(A->p);
    A->p = NULL;
}

#define ADDRESS(A, i, j) (A.p + A.stride * (A.y + i) + (A.x + j))

static int get(Mat A, int i, int j) {
    /* printf("xxx i=%d j=%d A.y=%d A.row=%d\n", i, j, A.y, A.row); */
    CHECK_RANGE(A, i, j);
    return *ADDRESS(A, i, j);
}

static void set(Mat A, int i, int j, int v) {
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

// A{m,n} * B[r, s] = R[m, s]
static void mat_mul(Mat A, Mat B, Mat C) {
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

static Mat mat_add_with_scaler(Mat A, Mat B, int k, Mat C) {
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

static Mat mat_add(Mat A, Mat B, Mat C) {
    return mat_add_with_scaler(A, B, 1, C);
}

static Mat mat_sub(Mat A, Mat B, Mat C) {
    return mat_add_with_scaler(A, B, -1, C);
}

static void mat_halve(Mat A, Mat *A11, Mat *A12, Mat *A21, Mat *A22) {
    // based on parent matrix's x, y, row, col, stride
    int halfrow = A.row / 2;
    int halfcol = A.col / 2;
    int x0 = A.x, x1 = A.x + halfcol;
    int y0 = A.y, y1 = A.y + halfrow;
    mat_init(A11, A.p, x0, y0, halfrow, halfcol, A.stride);
    mat_init(A12, A.p, x1, y0, halfrow, halfcol, A.stride);
    mat_init(A21, A.p, x0, y1, halfrow, halfcol, A.stride);
    mat_init(A22, A.p, x1, y1, halfrow, halfcol, A.stride);
}

typedef struct {
    Mat A, B, C;
    void *f;
    int depth;
} Arg;
void *mat_mul_async(void *arg) {
    Arg *p = arg;
    void (*fn)(Mat, Mat, Mat, void *, int) = p->f;
    fn(p->A, p->B, p->C, p->f, p->depth + 1);
    return arg;
}

static void *kall(pthread_t *thread,
                  Mat A, Mat B, Mat C, void *f, int depth) {
    Arg *arg = malloc(sizeof(Arg));
    arg->A = A;
    arg->B = B;
    arg->C = C;
    arg->f = f;
    arg->depth = depth;
    pthread_create(thread, NULL, mat_mul_async, arg);
    return NULL;
}

// we need one extra buffer to store temp matrix
// As the result C's memory is empty, actually we could reduce this number to 4.
// But it will make code complex.
#define M_NUM   8
#define CHECK_SQUARE(A) assert(A.row == A.col)
static void mat_mul_strassen_square_even(Mat A, Mat B, Mat C,
                                         void *f, int depth) {
    CHECK_MAT_MUL(A, B, C);
    CHECK_SQUARE(A);
    CHECK_SQUARE(B);
    CHECK_SQUARE(C);
    assert(A.row % 2 == 0);
    int half = A.row / 2;
    void (*fn)(Mat, Mat, Mat, void *, int) = f;
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

    // mat_show(A);
    // mat_show(A11); mat_show(A12); mat_show(A21); mat_show(A22);
    // mat_show(B);
    // mat_show(B11); mat_show(B12); mat_show(B21); mat_show(B22);

    // only need calling fn 7 times, but not 8 times
    if (depth == 0) {
#define THREAD_NUM  3
        pthread_t t[3];
        void *pret;
        kall(t, mat_sub(A12, A22, M[0]), mat_add(B21, B22, M[1]), M[7],
             fn, depth);
        kall(t + 1, mat_sub(A21, A11, M[0]), mat_add(B11, B12, M[1]), M[6],
             fn, depth);
        kall(t + 2, mat_add(A11, A22, M[0]), mat_add(B11, B22, M[2]), M[1],
             fn, depth);
        // leave this for main thread
        fn(mat_add(A11, A12, M[0]), B22, M[5], fn, depth);
        for (i = 0; i != THREAD_NUM; i++) {
            pthread_join(t[i], &pret);
            free(pret);
        }
        kall(t, A22, mat_sub(B21, B11, M[0]), M[4], fn, depth);
        kall(t + 1, A11, mat_sub(B12, B22, M[0]), M[3], fn, depth);
        kall(t + 2, mat_add(A21, A22, M[0]), B11, M[2], fn, depth);
        for (i = 0; i != THREAD_NUM; i++) {
            pthread_join(t[i], &pret);
            free(pret);
        }
    } else {
        fn(mat_sub(A12, A22, M[0]), mat_add(B21, B22, M[1]), M[7], fn, depth);
        fn(mat_sub(A21, A11, M[0]), mat_add(B11, B12, M[1]), M[6], fn, depth);
        fn(mat_add(A11, A22, M[0]), mat_add(B11, B22, M[2]), M[1], fn, depth);
        fn(mat_add(A11, A12, M[0]), B22, M[5], fn, depth);
        fn(A22, mat_sub(B21, B11, M[0]), M[4], fn, depth);
        fn(A11, mat_sub(B12, B22, M[0]), M[3], fn, depth);
        fn(mat_add(A21, A22, M[0]), B11, M[2], fn, depth);
    }

    mat_sub(mat_add(mat_add(M[7], M[4], C11), M[1], C11), M[5], C11);
    mat_add(M[3], M[5], C12);
    mat_add(M[2], M[4], C21);
    mat_sub(mat_add(mat_add(M[1], M[3], C22), M[6], C22), M[2], C22);

#if 0
    // embeded the test code
    Mat E;
    mat_init_buf(&E, A.row, A.row);
    mat_mul(A, B, E);
    if (mat_eq(E, C) != true) {
        printf("find error\n");
        mat_show(E);
        mat_show(C);
        assert(0);
    }
    mat_deinit_buf(&E);
#endif

    for (i = 0; i != M_NUM; i++) {
        mat_deinit_buf(&M[i]);
    }
}

static void mat_mul_strassen_square(Mat A, Mat B, Mat C, void *f, int depth) {
    assert(C.p != A.p && C.p != B.p);
    if (A.row != A.col) {
        assert(0); // TODO
    }
    if (A.row == 1) {
        set(C, 0, 0, get(A, 0, 0) * get(B, 0, 0));
    } else if (A.row % 2 != 0) {
        // padding zero and remove it in final result
        Mat Ap, Bp, Cp, Ac, Bc;
        int size = A.row + 1;
        mat_init_buf(&Ap, size, size);
        mat_init_buf(&Bp, size, size);
        mat_init_buf(&Cp, size, size);
        mat_init(&Ac, Ap.p, 0, 0, A.row, A.col, size);
        mat_init(&Bc, Bp.p, 0, 0, B.row, B.col, size);
        mat_add(A, Ac, Ac);
        mat_add(B, Bc, Bc);

        mat_mul_strassen_square_even(Ap, Bp, Cp, f, depth);

        int i, j;
        for (i = 0; i != C.row; i++) {
            for (j = 0; j != C.col; j++) {
                set(C, i, j, get(Cp, i, j));
            }
        }

        mat_deinit_buf(&Ap);
        mat_deinit_buf(&Bp);
        mat_deinit_buf(&Cp);
    } else {
        mat_mul_strassen_square_even(A, B, C, f, depth);
    }
}

void mat_mul_strassen(Mat A, Mat B, Mat C) {
    mat_mul_strassen_square(A, B, C, mat_mul_strassen_square, 1);
}

static void mat_mul_introspect_in(Mat A, Mat B, Mat C, void *f) {
    if (A.row != A.col || B.row != B.col) {
        assert(0); // not support
    }
    if (A.row <= 2048) {
        mat_mul(A, B, C);
    } else {
        mat_mul_strassen_square(A, B, C, mat_mul_introspect_in, 1);
    }
}

static void mat_mul_mt_in(Mat A, Mat B, Mat C, void *f, int depth) {
    if (A.row != A.col || B.row != B.col) {
        printf("A size=%dx%d B.size=%dx%d\n", A.row, A.col, B.row, B.col);
        assert(0); // not support
    }
    if (A.row <= 32) {
        mat_mul(A, B, C);
    } else {
        mat_mul_strassen_square(A, B, C, mat_mul_mt_in, depth);
    }
}

void mat_mul_introspect(Mat A, Mat B, Mat C) {
    return mat_mul_introspect_in(A, B, C, NULL);
}

void mat_mul_mt(Mat A, Mat B, Mat C) {
    return mat_mul_mt_in(A, B, C, NULL, 0);
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

void basic_2x2_1(void (*f)(Mat, Mat, Mat)) {
    int pa[] = {8, 3, 15, 9};
    int pb[] = {9, 0, 0, 0};
    int pc[4] = {0};
    int pe[4] = {72, 0, 135, 0};
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

void basic_3x3(void (*f)(Mat, Mat, Mat)) {
    int pa[] = {1, 3, 7, 5, 9, 10, 9, 5, 7};
    int pb[] = {6, 8, 4, 2, 0, 11, 7, 5, 9};
    int pc[9] = {0};
    int pe[] = {61, 43, 100, 118, 90, 209, 113, 107, 154};
    Mat A, B, C, E;
    mat_init(&A, pa, 0, 0, 3, 3, 3);
    mat_init(&B, pb, 0, 0, 3, 3, 3);
    mat_init(&C, pc, 0, 0, 3, 3, 3);
    mat_init(&E, pe, 0, 0, 3, 3, 3);
    mat_show(A);
    mat_show(B);
    f(A, B, C);
    mat_show(C);
    assert(mat_eq(C, E) == true);
}

void basic_4x4(void (*f)(Mat, Mat, Mat)) {
    int pa[] = {1, 3, 7, 0, 5, 9, 10, 0, 9, 5, 7, 0, 0, 0, 0, 0};
    int pb[] = {6, 8, 4, 0, 2, 0, 11, 0, 7, 5, 9, 0, 0, 0, 0, 0};
    int pc[16] = {0};
    int pe[] = {61, 43, 100, 0, 118, 90, 209, 0, 113, 107, 154, 0, 0, 0, 0, 0};
    Mat A, B, C, E;
    mat_init(&A, pa, 0, 0, 4, 4, 4);
    mat_init(&B, pb, 0, 0, 4, 4, 4);
    mat_init(&C, pc, 0, 0, 4, 4, 4);
    mat_init(&E, pe, 0, 0, 4, 4, 4);
    mat_show(A);
    mat_show(B);
    f(A, B, C);
    mat_show(C);
    assert(mat_eq(C, E) == true);
}

void test_group(void (*f)(Mat, Mat, Mat)) {
    basic_2x2(f);
    basic_2x2_1(f);
    basic_3x3(f);
    basic_4x4(f);
}

void mat_init_buf_rnd(Mat *A, int row, int col) {
    mat_init_buf(A, row, col);
    int i, j;
    for (i = 0; i != row; i++) {
        for (j = 0; j != col; j++) {
            int range = 16;
            set(*A, i, j, rand() % range - range / 2);
        }
    }
}

#define LOG(...)                 \
    fprintf(stdin, __VA_ARGS__); \
    fprintf(stderr, __VA_ARGS__)

void perf(void (*f)(Mat, Mat, Mat), int sz) {
    Mat A, B, C;
    mat_init_buf_rnd(&A, sz, sz);
    mat_init_buf_rnd(&B, sz, sz);
    mat_init_buf(&C, sz, sz);
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    f(A, B, C);
    gettimeofday(&stop, NULL);
    LOG("%lu",
        (stop.tv_sec - start.tv_sec) * 1000 * 1000 +
        stop.tv_usec - start.tv_usec);
    mat_deinit_buf(&A);
    mat_deinit_buf(&B);
    mat_deinit_buf(&C);
    // mat_show(C);

#if 0
    Mat E;
    mat_init_buf(&E, sz, sz);
    mat_mul(A, B, E);
    assert(mat_eq(C, E) == true);
    mat_deinit_buf(E);
#endif
}

void basic_test() {
    basic_2x3(mat_mul);
    test_group(mat_mul);
    test_group(mat_mul_strassen);
    test_group(mat_mul_introspect);
    test_group(mat_mul_mt);
}

void perf_test() {
    LOG("N\tnaive\tstrassen\n");
    int i;
    for (i = 2; i != 1024 * 8; i *= 2) {
        LOG("%d\t", i);
        perf(mat_mul_mt, i);
        // printf("\t");
        // perf(mat_mul, i);
        // LOG("\t");
        // perf(mat_mul_strassen, i);
        LOG("\n");
        fflush(stdin);
    }
}

int main() {
    basic_test();
    perf_test();
    return 0;
}
