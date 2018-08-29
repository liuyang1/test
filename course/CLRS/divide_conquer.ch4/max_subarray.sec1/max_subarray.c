#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <sys/time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static int add_overflow(int a, int b) {
#if 1
    return a + b;
#else
    int s = a + b;
    if ((a > 0 && b > 0 && (s < a || s < b)) ||
        (a < 0 && b < 0 && (s > a || s > b))) {
        printf("%d + %d = %d overflow!!!\n", a, b, s);
    }
    return s;
#endif
}

int brute_force(int *a, int n) {
    assert(n >= 1);
    int i, j, s, m;
    for (i = 0, m = a[0]; i != n; i++) {
        for (j = i, s = 0; j != n; j++) {
            s = add_overflow(s, a[j]);
            m = MAX(s, m);
        }
    }
    return m;
}

static int recursive_in(int *a, int low, int high, void *fn) {
    if (low == high) {
        return a[low];
    }
    assert(low < high);
    int (*f)(int *, int, int, void *) = fn;
    int mid = (low + high) / 2;
    int lm = f(a, low, mid, f);
    int rm = f(a, mid + 1, high, f);

    int mm = 0;
    int i, s, m1, m2;
    for (i = mid - 1, s = m1 = a[mid]; i >= low; i--) {
        s = add_overflow(a[i], s);
        m1 = MAX(s, m1);
    }
    for (i = mid + 1, s = m2 = a[mid]; i <= high; i++) {
        s = add_overflow(a[i], s);
        m2 = MAX(s, m2);
    }
    mm = m1 + m2 - a[mid];

    lm = MAX(lm, rm);
    return MAX(lm, mm);
}

int recursive(int *a, int n) {
    return recursive_in(a, 0, n - 1, recursive_in);
}

#define mix_in_M(thresh)                                          \
    static int mix_in_ ## thresh(int *a, int low, int high) {     \
        int len = high - low + 1;                                 \
        if (len <= thresh) {                                      \
            return brute_force(a + low, len);                     \
        } else {                                                  \
            return recursive_in(a, low, high, mix_in_ ## thresh); \
        }                                                         \
    }                                                             \
    int mix_ ## thresh(int *a, int n) {                           \
        return mix_in_ ## thresh(a, 0, n - 1);                    \
    }

mix_in_M(2);
mix_in_M(4);
mix_in_M(6);
mix_in_M(8);
mix_in_M(10);
mix_in_M(16);

int kadane(int *a, int n) {
    int i, s, m;
    for (i = 1, s = m = a[0]; i != n; i++) {
        s = MAX(a[i], s + a[i]);
        m = MAX(s, m);
    }
    return m;
}

/****************** TEST CODE ************************************************/
void test(int (*f)(int *, int)) {
    int a[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int v = f(a, sizeof(a) / sizeof(a[0]));
    printf("%d\n", v);
}

void gen_data(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        // rand() return [0, RNAD_MAX], move to [-RAND_MAX/2, RNAD_MAX/2]
        // move to [-1024, 1024] to avoid overflow
        a[i] = (rand() - RAND_MAX / 2) % 1024;
    }
}

int perf_on_func(int (*f)(int *, int), int *a, int n, bool last) {
    /* gen_data(a, n); */
    int v0 = 0;
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    for (int i = 0; i != 64; i++) {
        v0 = f(a, n);
    }
    gettimeofday(&stop, NULL);
    printf("%lu",
           (stop.tv_sec - start.tv_sec) * 1000 * 1000 +
           stop.tv_usec - start.tv_usec);
    printf(last ? "\n" : "\t");
    return v0;
}

void assert_check(int a, int b) {
    if (a != b) {
        printf("assert on %d != %d\n", a, b);
        assert(0);
    }
}

void perf(int n) {
    int *a = malloc(sizeof(int) * n);
    gen_data(a, n);
    printf("%d\t", n);
    int v0 = perf_on_func(brute_force, a, n, false);
    int v = perf_on_func(recursive, a, n, false);
    assert_check(v, v0);
    v = perf_on_func(mix_2, a, n, false);
    assert_check(v, v0);
    v = perf_on_func(mix_4, a, n, false);
    assert_check(v, v0);
    v =  perf_on_func(mix_8, a, n, false);
    assert_check(v, v0);
    v = perf_on_func(mix_16, a, n, false);
    assert_check(v, v0);
    v = perf_on_func(kadane, a, n, true);
    assert_check(v, v0);
    free(a);
    /* printf("%d %d %d\n", v0, v1, v2); */
}

void basic_test() {
    test(brute_force);
    test(recursive);
    test(mix_2);
    test(mix_4);
    test(mix_6);
    test(mix_8);
    test(mix_10);
    test(mix_16);
    test(kadane);
}

void perf_test() {
    printf("N\tbrute\trecur\tmix_2\tmix_4\tmix_8\tmix_16\tkadane\n");
    int n;
    for (n = 2; n != 32; n += 1) {
        perf(n);
    }
    for (; n < 1024; n += n / 4) {
        perf(n);
    }
}

int main() {
    /* basic_test(); */
    perf_test();
    return 0;
}
