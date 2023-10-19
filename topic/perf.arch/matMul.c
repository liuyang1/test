/**
 * optimization on Intel platform
 * matrix multiplication
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <x86intrin.h> // all sse/avx headers
#include <immintrin.h>
#include <smmintrin.h>
// #include <avx2intrin.h>

#include <time.h>

#define T int
#define fmt_T "%d"

typedef struct {
    T *d;
    size_t w;
    bool transed;
} Mat;

#define IDX(m, i, j) (m->d[i * m->w + j])

/** crash when aligned_alloc
 * good when posix_memalign */
void *my_malloc(size_t sz) {
    // void *p = (void *)aligned_alloc(32, sz);
    void *p;
    int r = posix_memalign(&p, 32, sz);
    if (r == 0 && p) {
        return p;
    }
    return NULL;
}

void my_memcpy(void *dst, void *src, size_t sz) {
    if (sz % (sizeof(int) * 8) == 0) {
        T *d = (T *)dst, *s = (T *)src;
        size_t i;
        for (i = 0; i != sz / sizeof(int); i += 8) {
            __m256i m = _mm256_loadu_si256((__m256i*)(s + i));
            _mm256_storeu_si256((__m256i*)(d + i), m);
        }
        return;
    }
    assert(0);
}

Mat *mat_init(T *a, size_t w) {
    Mat *m = malloc(sizeof(Mat));
    size_t size = sizeof(T) * w * w;
    m->d = my_malloc(size);
    my_memcpy(m->d, a, size);
    m->w = w;
    m->transed = false;
    return m;
}

Mat *mat_init_zero(size_t w) {
    size_t size = sizeof(T) * w * w;
    T *d = my_malloc(size);
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

void mat_mul_ikj(Mat *a, Mat *b, Mat *c) {
    size_t w = a->w;
    size_t i, j, k;
    for (i = 0; i != w; i++) {
        for (k = 0; k != w; k++) {
            T t = IDX(a, i, k);
            for (j = 0; j != w; j++) {
                IDX(c, i, j) += t * IDX(b, k, j);
            }
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

void show_8_epi32(T *a, int end) {
    printf("%d %d %d %d  %d %d %d %d\t",
           a[0], a[1], a[2], a[3],
           a[4], a[5], a[6], a[7]);
    printf("%c", end ? '\n' : ' ');
}

void mm256_show(__m256i *m, int end) {
    int a[8];
    _mm256_storeu_si256((__m256i*)a, *m);
    show_8_epi32(a, end);
}

T mul_add_16(T *a, T *b) {
    T t = 0;
    size_t i;
    for (i = 0; i != SM; i++) {
        t += a[i] * b[i];
    }
    return t;
}

T mul_add_16_intrin(T *a, T *b) {
    __m256i m1 = _mm256_load_si256((__m256i*)a);
    __m256i m2 = _mm256_load_si256((__m256i*)b);
    __m256i m = _mm256_mullo_epi32(m1, m2);
    __m256i n1 = _mm256_load_si256((__m256i*)&a[8]);
    __m256i n2 = _mm256_load_si256((__m256i*)&b[8]);
    __m256i n = _mm256_mullo_epi32(n1, n2);
    m = _mm256_add_epi32(m, n);
    __m256i sum = _mm256_add_epi32(m, _mm256_permute2x128_si256(m, m, 0x01));
    sum = _mm256_add_epi32(sum, _mm256_shuffle_epi32(sum, _MM_SHUFFLE(0, 0, 3, 2)));
    sum = _mm256_add_epi32(sum, _mm256_shuffle_epi32(sum, _MM_SHUFFLE(0, 0, 0, 1)));
    int res;
    _mm_storeu_si128((__m128i*)&res, _mm256_extracti128_si256(sum, 0));
    return res;
}

T mul_add_16_intrin1(__m256i m1, __m256i n1, T *b) {
    __m256i m2 = _mm256_stream_load_si256((__m256i*)b);
    __m256i m = _mm256_mullo_epi32(m1, m2);
    __m256i n2 = _mm256_stream_load_si256((__m256i*)&b[8]);
    __m256i n = _mm256_mullo_epi32(n1, n2);
    m = _mm256_add_epi32(m, n);
    // fast than naive loop
    // a0 a1 a2 a3, a4 a5 a6 a7 + a4 a5 a6 a7, a0 a1 a2 a3
    __m256i sum = _mm256_add_epi32(m, _mm256_permute2x128_si256(m, m, 0x01));
    // a0+a4 a1+a5 a2+a6 a3+a7  | a0+a4 a1+a5 a2+a6 a3+a7
    // _MM_SHUFFLE(0, 0, 3, 2)); 0xe = 0b00001110
    // 10 11 00 00   10 11 00 00
    // a2+a6 a3+a7 a0+a4 a0+a4  | a2+a6 a3+a7 a0+a4 a0+a4
    sum = _mm256_add_epi32(sum, _mm256_shuffle_epi32(sum, _MM_SHUFFLE(0, 0, 3, 2)));
    // a0+a2+a4+a6 a1+a3+a5+a7 ...
    // 01 00 00 00   00 00 00 00
    // a1+a3+a5+a7 ...
    sum = _mm256_add_epi32(sum, _mm256_shuffle_epi32(sum, _MM_SHUFFLE(0, 0, 0, 1)));
    // a0+...a7
    int res;
    _mm_storeu_si128((__m128i*)&res, _mm256_extracti128_si256(sum, 0));
    return res;
}

void mat_mul_subm_intrin(Mat *a, Mat *b, Mat *c) {
    mat_transpose(b);
    size_t w = a->w;
    size_t i, j, k, i2, k2;
    T *rres, *rmul1, *rmul2;
    for (i = 0; i < w; i += SM) {
        for (j = 0; j < w; j += SM) {
            for (k = 0; k < w; k += SM) {
                for (i2 = 0, rres = &(IDX(c, i, j)), rmul1 = &(IDX(a, i, k));
                     i2 < SM;
                     i2++, rres += w, rmul1 += w) {
                    for (k2 = 0, rmul2 = &(IDX(b, j, k));
                         k2 < SM;
                         k2++, rmul2 += w) {
                        // rres[k2] += mul_add_16(rmul1, rmul2);
                        rres[k2] += mul_add_16_intrin(rmul1, rmul2);
                    }
                }
            }
        }
    }
}

void mat_mul_subm_intrin1(Mat *a, Mat *b, Mat *c) {
    mat_transpose(b);
    size_t w = a->w;
    size_t i, j, k, i2, k2;
    T *rres, *rmul1, *rmul2;
    for (i = 0; i < w; i += SM) {
        for (j = 0; j < w; j += SM) {
            for (k = 0; k < w; k += SM) {
                for (i2 = 0, rres = &(IDX(c, i, j)), rmul1 = &(IDX(a, i, k));
                     i2 < SM;
                     i2++, rres += w, rmul1 += w) {
                    __m256i m1 = _mm256_stream_load_si256((__m256i*)rmul1);
                    __m256i n1 = _mm256_stream_load_si256((__m256i*)&rmul1[8]);
                    for (k2 = 0, rmul2 = &(IDX(b, j, k));
                         k2 < SM;
                         k2++, rmul2 += w) {
                        // rres[k2] += mul_add_16(rmul1, rmul2);
                        rres[k2] += mul_add_16_intrin1(m1, n1, rmul2);
                    }
                }
            }
        }
    }
}

#define XM  (256/sizeof(T)/8) // 256/4/8=8
void mat_mul_ikj_intrin(Mat *a, Mat *b, Mat *c) {
    size_t w = a->w;
    size_t i, j, k, i2, k2;
    assert(a->w % XM == 0);
    for (i = 0; i != w; i++) {
        for (k = 0; k != w; k++) {
            T t = IDX(a, i, k);
            __m256i aa = _mm256_set1_epi32(t);
            // printf("i=%d k=%d t=%d\n", i, k, t);
            for (j = 0; j != w; j += XM) {
                // cc += aa * bb
                __m256i bb = _mm256_stream_load_si256((__m256i*)(&IDX(b, k, j)));
                __m256i cc = _mm256_stream_load_si256((__m256i*)&IDX(c, i, j));
                __m256i r = _mm256_mullo_epi32(bb, aa);
                // mm256_show(&aa, 0), putchar('*'), mm256_show(&bb, 0), putchar('='), mm256_show(&r, 1);
                // mm256_show(&cc, 0), putchar('+'), mm256_show(&r, 1);
                cc = _mm256_add_epi32(cc, r);
                // mm256_show(&cc, 1);
                _mm256_stream_si256((__m256i*)&IDX(c, i, j), cc);
            }
        }
    }
}


static inline void mat_mul(Mat *a, Mat *b, Mat *c) {
    assert(a->w == b->w && a->w == c->w);
    // mat_mul_naive(a, b, c);
    // mat_mul_mem(a, b, c);
    // mat_mul_subm(a, b, c);
    // mat_mul_subm_intrin(a, b, c);
    // mat_mul_subm_intrin1(a, b, c);
    // mat_mul_ikj(a, b, c);
    mat_mul_ikj_intrin(a, b, c);
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
    T *d0 = my_malloc(size), *d1 = my_malloc(size);
    size_t i;
    for (i = 0; i != w * w; i++) {
        d0[i] = rand() % 2;
        d1[i] = rand() % 2;
    }
    Mat *a = mat_init(d0, w);
    Mat *b = mat_init(d1, w);
    free(d0); free(d1);
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
    T *d0 = my_malloc(size), *d1 = my_malloc(size);
    size_t i;
    for (i = 0; i != w * w; i += 8) {
        d0[i] = rand() % 2;
        d1[i] = rand() % 2;
    }
    Mat *a = mat_init(d0, w), *b = mat_init(d1, w);
    Mat *b_origin = mat_init(d1, w);
    Mat *c0 = mat_init_zero(w);
    Mat *c1 = mat_init_zero(w);
    Mat *c2 = mat_init_zero(w);

    mat_mul_naive(a, b, c0);
    mat_mul_mem(a, b, c1);
    // mat_mul_subm_intrin1(a, b, c2);
    mat_mul_ikj_intrin(a, b_origin, c2);
    // mat_mul_ikj(a, b_origin, c2);
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
    free(d0), free(d1);
    mat_deinit(c0), mat_deinit(c1), mat_deinit(c2);
    return r;
}

int main() {
    printf("sizeof(int)=%lu\n", sizeof(T));
#if 0
    test_mul_4();
#endif
    printf("test with gold\n");
    if (!test_gold(16)) {
        return -1;
    }
    if (!test_gold(32)) {
        return -1;
    }
    size_t w;
    for (w = 32; w <= 4096; w *= 2) {
        printf("test width=%zu\n", w);
        test_perf_width(w);
    }
    return 0;
}
