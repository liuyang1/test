/**
 * optimization on Intel platform
 * matrix multiplication
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <time.h>

#define T int
#define fmt_T "%d"

typedef struct {
    T *d;
    size_t w;
    bool transed;
} Mat;

#define IDX(m, i, j) (m->d[i * m->w + j])

Mat *mat_init(T *a, size_t w) {
    Mat *m = malloc(sizeof(Mat));
    size_t size = sizeof(T) * w * w;
    m->d = malloc(size);
    memcpy(m->d, a, size);
    m->w = w;
    m->transed = false;
    return m;
}

Mat *mat_init_zero(size_t w) {
    size_t size = sizeof(T) * w * w;
    T *d = malloc(size);
    memset(d, 0, size);

    Mat *m = mat_init(d, w);

    free(d);
    return m;
}

void mat_deinit(Mat *m) {
    free(m->d);
    free(m);
}

void mat_transpose(Mat *m) {
    if (m->transed) {
        return;
    }
    size_t i, j;
    for (i = 0; i != m->w; i++) {
        for (j = i + 1; j != m->w; j++) {
            T t = IDX(m, i, j);
            IDX(m, i, j) = IDX(m, j, i);
            IDX(m, j, i) = t;
        }
    }
    m->transed = true;
}
bool mat_eq(Mat *a, Mat *b) {
    size_t size = sizeof(T) * a->w * a->w;
    if (a->w != b->w) {
        return false;
    }
    size_t i, j, w = a->w;
    for (i = 0; i != w; i++) {
        for (j = 0; j != w; j++) {
            if (IDX(a, i, j) == IDX(b, i, j)) {
            } else {
                printf("diff at (%zu, %zu) %d %d\n",
                       i, j, IDX(a, i, j), IDX(b, i, j));
                return false;
            }
        }
    }
    return true;
    return a->w == b->w && memcmp(a->d, b->d, size) == 0;
}

void mat_show(Mat *m) {
    printf("m=%p w=%zu transed=%s\n",
           m, m->w, m->transed ? "transed" : "origin");
    size_t i, j;
    for (i = 0; i != m->w; i++) {
        for (j = 0; j != m->w; j++) {
            printf(fmt_T", ", IDX(m, i, j));
        }
        printf("\n");
    }
}

/** naive version
 * simple loop
 */
void mat_mul_naive(Mat *a, Mat *b, Mat *c) {
    size_t w = a->w;
    size_t i, j, k;
    T t;
    for (i = 0; i != w; i++) {
        for (j = 0; j != w; j++) {
            t = 0;
            for (k = 0; k != w; k++) {
                t += IDX(a, i, k) * IDX(b, k, j);
            }
            IDX(c, i, j) = t;
        }
    }
}

void mat_mul_mem(Mat *a, Mat *b, Mat *c) {
    mat_transpose(b);
    size_t w = a->w;
    size_t i, j, k;
    T t;
    for (i = 0; i != w; i++) {
        for (j = 0; j != w; j++) {
            t = 0;
            for (k = 0; k != w; k++) {
                t += IDX(a, i, k) * IDX(b, j, k);
            }
            IDX(c, i, j) = t;
        }
    }
}

#define CLS 64 // getconf LEVLE1_DCACHE_LINESIZE 64
#define SM  (CLS/sizeof(T)) // 64/4=16

void mat_mul_subm(Mat *a, Mat *b, Mat *c) {
    mat_transpose(b);
    size_t w = a->w;
    size_t i, j, k, i2, j2, k2;
    T *rres, *rmul1, *rmul2;
    T t;
    for (i = 0; i < w; i += SM) {
        for (j = 0; j < w; j += SM) {
            for (k = 0; k < w; k += SM) {
                for (i2 = 0, rres = &(IDX(c, i, j)), rmul1 = &(IDX(a, i, k));
                     i2 < SM;
                     i2++, rres += w, rmul1 += w) {
                    for (k2 = 0, rmul2 = &(IDX(b, j, k));
                         k2 < SM;
                         k2++, rmul2 += w) {
                        T t = rres[k2];
                        for (j2 = 0; j2 < SM; j2++) {
                            t += rmul1[j2] * rmul2[j2];
                        }
                        rres[k2] = t;
                    }
                }
            }
        }
    }
}

static inline void mat_mul(Mat *a, Mat *b, Mat *c) {
    assert(a->w == b->w && a->w == c->w);
    // mat_mul_naive(a, b, c);
    // mat_mul_mem(a, b, c);
    mat_mul_subm(a, b, c);
}

/** test code ****************************************************************/
bool test_mul_4() {
    T d0[] = {1, 0, 0, 1,
        0, 0, 1, 1,
        0, 1, 1, 0,
        1, 0, 0, 0};
    T d1[] = {0, 1, 0, 1,
    1, 1, 0, 1,
    1, 0, 1, 0,
    0, 1, 1, 0};
    T d2[] = {0, 2, 1, 1,
    1, 1, 2, 0,
    2, 1, 1, 1,
    0, 1, 0, 1};
    size_t w = 4;
    Mat *a = mat_init(d0, w), *b = mat_init(d1, w), *e = mat_init(d2, w);
    Mat *c = mat_init_zero(w);
    mat_mul_mem(a, b, c);

    mat_show(a);
    printf("*\n");
    mat_show(b);
    printf("=\n");
    mat_show(c);

    if (mat_eq(c, e)) {
        printf("pass\n");
    } else {
        printf("fail, expect\n");
        mat_show(e);
    }

    mat_deinit(a);
    mat_deinit(b);
    mat_deinit(c);
    mat_deinit(e);

    return true;
}

int test_perf_width(size_t w) {
    size_t size = sizeof(T) * w * w;
    T *d0 = malloc(size), *d1 = malloc(size);
    size_t i;
    for (i = 0; i != w * w; i++) {
        d0[i] = rand() % 2;
        d1[i] = rand() % 2;
    }
    Mat *a = mat_init(d0, w), *b = mat_init(d1, w);
    Mat *c = mat_init_zero(w);

    clock_t t0 = clock();
    mat_mul(a, b, c);
    clock_t t1 = clock();
    printf("dt=%Lfus\n", (long double)(t1 - t0));

#if 0
    mat_show(a);
    printf("*\n");
    mat_show(b);
    printf("=\n");
    mat_show(c);
#endif

    mat_deinit(a), mat_deinit(b), mat_deinit(c);

    return 0;
}

int test_gold(size_t w) {
    size_t size = sizeof(T) * w * w;
    T *d0 = malloc(size), *d1 = malloc(size);
    size_t i;
    for (i = 0; i != w * w; i++) {
        d0[i] = rand() % 2;
        d1[i] = rand() % 2;
    }
    Mat *a = mat_init(d0, w), *b = mat_init(d1, w);
    Mat *c0 = mat_init_zero(w);
    Mat *c1 = mat_init_zero(w);
    Mat *c2 = mat_init_zero(w);

    mat_mul_naive(a, b, c0);
    mat_mul_mem(a, b, c1);
    mat_mul_subm(a, b, c2);
    bool r = mat_eq(c0, c1);
    printf("test_gold naive-mem w=%zu %s\n",
           w, r ? "pass" : "expect");
    if (!r) {
        mat_show(a); mat_show(b);
        mat_show(c0); mat_show(c1);
        return r;
    }
    r = mat_eq(c0, c2);
    printf("test_gold naive-submem w=%zu %s\n",
           w, r ? "pass" : "expect");
    if (!r) {
        mat_show(a); mat_show(b);
        printf("\n");
        mat_show(c0);
        printf("\n");
        mat_show(c2);
    }

    mat_deinit(a), mat_deinit(b);
    mat_deinit(c0), mat_deinit(c1), mat_deinit(c2);
    return r;
}

int main() {
    printf("sizeof(int)=%lu\n", sizeof(T));
    test_mul_4();
    printf("test with gold\n");
    if (!test_gold(16)) {
        return -1;
    }
    if (!test_gold(32)) {
        return -1;
    }
    size_t w;
    for (w = 16; w <= 4096; w *= 2) {
        printf("test width=%zu\n", w);
        test_perf_width(w);
    }
    return 0;
}
