#include "leet.h"
int combination(int n, int k) {
    if (n - k < n) {
        k = n - k;
    }
    int i, j;
    unsigned long long p0 = 1;
    for (i = n, j = 1; j != k + 1; i--, j++) {
        p0 = (i * p0) / j;
    }
    return p0;
}

typedef struct {
    int *st;
    int n;
    int k;
    int cnt;
} Ctx;

// n >= k
void initCtx(Ctx *ctx, int n, int k) {
    ctx->n = n;
    ctx->k = k;
    ctx->cnt = 0;
    ctx->st = malloc(sizeof(int) * k);
}
bool next(Ctx *ctx) {
    int i;
    if (ctx->cnt == 0) {
        for (i = 0; i != ctx->k; i++) {
            ctx->st[i] = i + 1;
        }
        ctx->cnt++;
        return true;
    }
    ctx->cnt++;

    // update from tail to head until not exceed to threshold, then reset it back
    // For example: 5 choose 3, iter to [1, 4, 5], then update to next
    // 1 4 5
    // 2(not thresh) 5(thresh) 6(thresh)
    // 2 3 4 reset it back
    for (i = ctx->k - 1; i >= 0; i--) {
        ctx->st[i]++;
        if (ctx->st[i] - i + (ctx->k - 1) != ctx->n + 1) {
            break;
        }
    }
    if (i < 0) {
        // iter to head and cannot find one below thresh, so iter to end
        // For example: 5 choose 3, iter to [3, 4, 5], then update to
        // [4(t), 5(t), 6(t)]
        return false;
    }
    for (i += 1; i != ctx->k; i++) {
        ctx->st[i] = ctx->st[i - 1] + 1;
    }
    return true;
}

void snapshot(Ctx *ctx, int *save) {
    int i;
    for (i = 0; i != ctx->k; i++) {
        save[i] = ctx->st[i];
    }
}

void finitCtx(Ctx *ctx) {
    free(ctx->st);
}

int** combine(int n, int k, int** columnSizes, int* returnSize) {
    Ctx ctx, *p = &ctx;
    initCtx(p, n, k);

    int cmb = combination(n, k), i;
    int *colsz = malloc(sizeof(int) * cmb);
    int **r = malloc(sizeof(int *) * cmb);
    for (i = 0; i != cmb; i++) {
        colsz[i] = k;
        r[i] = malloc(sizeof(int) * k);
        next(p);
        snapshot(p, r[i]);
    }

    finitCtx(p);

    *returnSize = cmb;
    *columnSizes = colsz;
    return r;
}

int testCtx() {
    Ctx ctx, *p = &ctx;
    initCtx(p, 6, 4);
    while(next(p)) {
        printf("%d:\t", p->cnt);
        showArr(p->st, 4);
    }
    return 0;
}
int testCmb(int n, int k) {
    int *colsz, retsz;
    int **r = combine(n, k, &colsz, &retsz);
    int i;
    printf("retsz=%d\n", retsz);
    for (i = 0; i != retsz; i++) {
        printf("%d:\t", i);
        showArr(r[i], colsz[i]);
        free(r[i]);
    }
    if (r) {
        free(r);
    }
    return 0;
}

int main() {
    testCmb(1, 1);
    testCmb(6, 4);
    return 0;
}
