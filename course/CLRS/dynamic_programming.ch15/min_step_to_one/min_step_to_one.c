#include <stdio.h>
#include <stdlib.h>

void show_arr(int *a, int size) {
    int i;
    for (i = 0; i != size; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
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
    printf("%d %d %f\n", j, n, j / (n + 0.0));
    free(cand);
    int ret = r[n];
    free(r);
    return ret;
}

int main(int argc, char **argv) {
    int n;
    if (argc == 1) {
        n = 1000;
        printf("default to %d\n", n);
    } else {
        n = atoi(argv[1]);
    }
    int step = min_step_to_one(n);
    // printf("step from %d to 1, at least need %d steps\n", n, step);
    return 0;
}
