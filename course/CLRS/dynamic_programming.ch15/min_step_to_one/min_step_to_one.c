#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#include <sys/time.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void show_arr(int *a, int size) {
    int i;
    printf("[");
    for (i = 0; i != size; i++) {
        printf("%d%s", a[i], (i + 1) == size ? "]\n" : ",");
    }
}

int min_step_to_one_memo_in(int *memo, int n) {
    if (n == 1) {
        return 0;
    }
    if (memo[n] != 0) {
        return memo[n];
    }
    int m = min_step_to_one_memo_in(memo, n - 1), v;
    if (n % 2 == 0) {
        v = min_step_to_one_memo_in(memo, n / 2);
        m = MIN(m, v);
    }
    if (n % 3 == 0) {
        v = min_step_to_one_memo_in(memo, n / 3);
        m = MIN(m, v);
    }
    m++;
    memo[n] = m;
    return m;
}

int min_step_to_one_memo(int n) {
    int *memo = malloc(sizeof(int) * (n + 1));
    bzero(memo, sizeof(int) * (n + 1));
    int ret = min_step_to_one_memo_in(memo, n);
    free(memo);
    return ret;
}

// slow naive solution
// T(n) = T(n - 1) + T(n / 2) + T(n / 3) + O(1)
int min_step_to_one_naive(int n) {
    if (n == 1) {
        return 0;
    }
    int m = min_step_to_one_naive(n - 1), v;
    if (n % 2 == 0) {
        v = min_step_to_one_naive(n / 2);
        m = MIN(m, v);
    }
    if (n % 3 == 0) {
        v = min_step_to_one_naive(n / 3);
        m = MIN(m, v);
    }
    return m + 1;
}

int min_step_to_one(int n) {
    int *r = malloc(sizeof(int) * (n + 1));
    int i, j;
    for (i = 0; i != n + 1; i++) {
        r[i] = -1;
    }
    int *cand = malloc(sizeof(int) * n);

    cand[0] = 1; // start from one (bottom-up)
    r[1] = 0; // step from one to one is zero
    for (i = 0, j = 1; i != n; i++) {
        int c = cand[i];
        int next = r[c] + 1;

#define CHECK_UPDATE(v)                          \
    if (v <= n && r[v] < 0) {                    \
        r[v] = next;                             \
        if (j < n) { cand[j++] = v; }            \
        if (v == n) { goto end;} /* quick end */ \
    }

        int v = c + 1;
        CHECK_UPDATE(v);
        v = c * 2;
        CHECK_UPDATE(v);
        v = c * 3;
        CHECK_UPDATE(v);
    }

end:
    // show_arr(r, n + 1);
    // show_arr(cand, j);
    // printf("%d %d %f\n", j, n, j / (n + 0.0));
    free(cand);
    int ret = r[n];
    free(r);
    return ret;
}

static unsigned long long diffTime(struct timeval t0, struct timeval t1) {
    return (t0.tv_sec - t1.tv_sec) * 1000 * 1000 + t0.tv_usec - t1.tv_usec;
}

int perf_test() {
    int i, j;
    for (i = 1000; i <= 1000 * 1000; i *= 2) {
        long long unsigned t0 = 0, t1 = 0;
        const int range = 100;
        for (j = 0; j != range; j++) {
            int n  = i + j;
            struct timeval start, stop;

            gettimeofday(&start, NULL);
            int s1 = min_step_to_one(n);
            gettimeofday(&stop, NULL);
            t0 += diffTime(stop, start);

            gettimeofday(&start, NULL);
            int s2 = min_step_to_one_memo(n);
            gettimeofday(&stop, NULL);
            t1 += diffTime(stop, start);
            assert(s1 == s2);
        }
        double mt0 = t0 / (range + 0.0);
        double mt1 = t1 / (range + 0.0);
        printf("%d\t%f\t%f\t%f\n", i, mt0, mt1, mt0 / mt1);
    }
    return 0;
}

int main(int argc, char **argv) {
#if 1
    return perf_test();
#else
    int n;
    if (argc == 1) {
        n = 1000;
    } else {
        n = atoi(argv[1]);
    }
    int step = min_step_to_one(n);
    printf("step from %d to 1, at least need %d steps\n", n, step);
    return 0;
#endif
}
