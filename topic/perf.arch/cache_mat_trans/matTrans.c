/**
 * optimization on Intel platform
 * matrix multiplication
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include <time.h>

#define T int
#define fmt_T "%d"

typedef struct {
    T *d;
    size_t w;
    bool transed;
} Mat;

#define IDX(m, i, j) (m->d[(i) * m->w + (j)])

/** crash when aligned_alloc
 * good when posix_memalign */
void *my_malloc(size_t sz) {
    // void *p = (void *)aligned_alloc(32, sz);
    void *p;
    int r = posix_memalign(&p, 64, sz);
    if (r == 0 && p) {
        return p;
    }
    return NULL;
}

Mat *mat_init(T *a, size_t w) {
    Mat *m = malloc(sizeof(Mat));
    size_t size = sizeof(T) * w * w;
    m->d = my_malloc(size);
    memcpy(m->d, a, size);
    // printf("m->d=%p\n", m->d);
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
            // printf("%zu,%zu %d %d\n", i, j, IDX(m, i, j), IDX(m, j, i));
            T t = IDX(m, i, j);
            IDX(m, i, j) = IDX(m, j, i);
            IDX(m, j, i) = t;
        }
    }
    m->transed = true;
}

void mat_transpose_blk(Mat *m, size_t sub) {
    if (m->transed) {
        return;
    }
    if (m->w % sub != 0) {
        assert(0);
    }
    size_t i, j, u, v;
#if 0
    for (i = 0; i != m->w; i += sub) {
        for (j = 0; j != m->w; j += sub) {
            for (u = 0; u != sub; u++) {
                for (v = 0; v != sub; v++) {
                    if (j + v > i + u) {
                        // printf("%zu,%zu %d %d\n", i+u, j+v, IDX(m, i + u, j + v), IDX(m, j + v, i + u));
                        T t = IDX(m, i + u, j + v);
                        IDX(m, i + u, j + v) = IDX(m, j + v, i + u);
                        IDX(m, j + v, i + u) = t;
                    }
                }
            }
        }
    }
#else
    for (i = 0; i != m->w; i += sub) {
        j = i;
        for (u = 0; u != sub; u++) {
            for (v = u + 1; v != sub; v++) {
                printf("(%zu,%zu),", i+u, j+v);
                T t = IDX(m, i + u, j + v);
                IDX(m, i + u, j + v) = IDX(m, j + v, i + u);
                IDX(m, j + v, i + u) = t;
            }
        }
        for (j += sub; j != m->w; j += sub) {
            for (u = 0; u != sub; u++) {
                for (v = 0; v != sub; v++) {
                    printf("(%zu,%zu),", i+u, j+v);
                    T t = IDX(m, i + u, j + v);
                    IDX(m, i + u, j + v) = IDX(m, j + v, i + u);
                    IDX(m, j + v, i + u) = t;
                }
            }
        }
        printf("\n");
    }
#endif
    m->transed = true;
}

void mat_transpose_blk_odd(Mat *m, size_t sub) {
    if (m->transed) {
        return;
    }
    size_t i, j, u, v;
    for (i = 0; i < m->w; i += sub) {
        for (j = 0; j < m->w; j += sub) {
            for (u = 0; u != sub; u++) {
                for (v = 0; v != sub; v++) {
                    if (j + v > i + u && j + v < m->w && i + u < m->w) {
                        T t = IDX(m, i + u, j + v);
                        IDX(m, i + u, j + v) = IDX(m, j + v, i + u);
                        IDX(m, j + v, i + u) = t;
                    }
                }
            }
        }
    }
    m->transed = true;
}

#define SWAP(m, a, b) { T t = IDX(m, a, b); IDX(m, a, b) = IDX(m, b, a); IDX(m, b, a) = t; }

void mat_transpose_cached_blk(Mat *m, size_t sub) {
    if (m->transed) { return; }
    size_t i, j, u, v;
    for (i = 0; i != m->w; i += sub) {
        j = i;
        // swap diag
        for (u = 0; u != sub; u++) {
            for (v = u + 1; v != sub; v++) {
                SWAP(m, i + u, j + v);
            }
        }
    }
    T *buf = malloc(sizeof(T) * sub * sub);
    for (i = 0; i != m->w; i += sub) {
        for (j = i + sub; j != m->w; j += sub) {
            // for (u = 0; u != sub; u++) {
            //     for (v = 0; v != sub; v++) {
            //         SWAP(m, i + u, j + v);
            //     }
            // }
#if 1
            for (u = 0; u != sub; u++) {
                // for (v = 0; v != sub; v++) {
                //     buf[u * sub + v] = IDX(m, i + u, j + v);
                // }
                memcpy(buf + u * sub, &IDX(m, i + u, j), sizeof(T) * sub);
            }
            for (u = 0; u != sub; u++) {
                for (v = 0; v != sub; v++) {
                    IDX(m, i + u, j + v) = IDX(m, j + v, i + u);
                }
            }
            for (v = 0; v != sub; v++) {
                for (u = 0; u != sub; u++) {
                    IDX(m, j + v, i + u) = buf[u * sub + v];
                }
            }
#endif
        }
    }
    free(buf);
    m->transed = true;
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

void mat_vis(Mat *m) {
    printf("m=%p w=%zu transed=%s\n",
           m, m->w, m->transed ? "transed" : "origin");
    size_t i, j;
    for (i = 0; i != m->w; i++) {
        for (j = 0; j != m->w; j++) {
            printf("%c ", IDX(m, i, j) ? '1':' ');
        }
        printf("\n");
    }
}

Mat *copy_mat(Mat *m) {
    Mat *r = mat_init(m->d, m->w);
    r->transed = m->transed;
    return r;
}

Mat *create_rand_mat(size_t w) {
    size_t size = sizeof(T) * w * w;
    T *d = malloc(size);
    size_t i;
    for (i = 0; i != w * w; i++) {
        d[i] = rand() % 2;
    }
    Mat *a = mat_init(d, w);
    free(d);
    return a;
}

bool mat_eq(Mat *a, Mat *b) {
    size_t size = sizeof(T) * a->w * a->w;
    if (a->w != b->w) {
        return false;
    }
    if (a->transed != b->transed) {
        printf("transed a=%d b=%d\n", a->transed, b->transed);
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


int test_perf_width(size_t w) {
    Mat *a = create_rand_mat(w);
    Mat *o = copy_mat(a);
    size_t wrr[] = {4, 8, 16, 32};

    clock_t t0 = clock();
    mat_transpose(a);
    clock_t t1 = clock();
    printf("(%.Lf,", (long double)(t1 - t0));

    size_t i;
    for (i = 0; i != sizeof(wrr) / sizeof(wrr[0]); i++) {
        Mat *m = copy_mat(o);
        // mat_vis(m);

        t0 = clock();
        // mat_transpose_blk(m, wrr[i]);
        mat_transpose_cached_blk(m, wrr[i]);
        t1 = clock();
        // mat_vis(m);
        printf("%.Lf,", (long double)(t1 - t0));
        if (!mat_eq(m, a)) {
            printf("\n%zu\n", wrr[i]);
            mat_vis(a);
            mat_vis(m);
            assert(0);
        }
        mat_deinit(m);
    }
    printf("),\n");

    mat_deinit(a);
    mat_deinit(o);

    return 0;
}

int test_gold(size_t w) {
    Mat *m = create_rand_mat(w);
    Mat *n = copy_mat(m);
    Mat *n1 = copy_mat(m);
    mat_vis(m);
    mat_vis(n);
    mat_transpose(m);
    mat_transpose_blk(n, 2);
    mat_transpose_cached_blk(n1, 2);
    if (!mat_eq(m, n)) {
        mat_vis(m);
        mat_vis(n);
        printf("DIFF\n");
        assert(0);
    }
    if (!mat_eq(m, n1)) {
        mat_vis(m);
        mat_vis(n1);
        printf("DIFF\n");
        assert(0);
    }

    mat_deinit(m);
    mat_deinit(n); mat_deinit(n1);
    return 0;
}

int main() {
#if 1
    Mat *m = create_rand_mat(16);
    mat_transpose_blk(m, 2);
    return 0;
    test_gold(4);
    test_gold(8);
    test_gold(16);
    test_gold(32);
    return 0;
#else
    size_t w;
    for (w = 32; w <= 4096 * 8; w *= 2) {
        // printf("test width=%zu\n", w);
        test_perf_width(w);
    }
    return 0;
#endif
}
