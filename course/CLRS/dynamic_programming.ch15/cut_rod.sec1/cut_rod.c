#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// length from 1 to 10
int tbl[10] = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};

typedef struct {
    int *a;
    int n;
} Ctx;

Ctx *ctx_init(int n) {
    Ctx *p = malloc(sizeof(Ctx));
    p->a = malloc(sizeof(int) * (n + 1));
    bzero(p->a, sizeof(int) * (n + 1));
    p->n = n;
    return p;
}

void ctx_deinit(Ctx *p) {
    free(p->a);
    free(p);
}

int ctx_get(Ctx *p, int n) {
    if (n > p->n && n >= 1) {
        printf("n=%d is greater than ctx size=%d\n", n, p->n);
        assert(0);
    }
    return p->a[n];
}
void ctx_set(Ctx *p, int n, int v) {
    if (n > p->n && n >= 1) {
        printf("n=%d is greater than ctx size=%d\n", n, p->n);
        assert(0);
    }
    p->a[n] = v;
}

int cut_rod_in(Ctx *p, int n) {
    int v = ctx_get(p, n);
    if (n == 0) {
        return 0;
    }
    if (v != 0) {
        return v;
    }
    int i, m;
    for (i = 1, m = 0; i <= sizeof(tbl) / sizeof(tbl[0]); i++) {
        if (n - i < 0) {
            break;
        }
        v = tbl[i - 1] + cut_rod_in(p, n - i);
        m = MAX(m, v);
    }
    ctx_set(p, n, m);
    return m;
}

int cut_rod(int m) {
    Ctx *p = ctx_init(m);
    int v = cut_rod_in(p, m);
    int i;
    for (i = 1; i != m + 1; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 1; i != m + 1; i++) {
        printf("%d " , ctx_get(p, i));
    }
    printf("\n");
    ctx_deinit(p);
    return v;
}

int main(int argc, char **argv) {
    cut_rod(20);
    return 0;
}
