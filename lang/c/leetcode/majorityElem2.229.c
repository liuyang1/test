#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1 // to pass compile under pure C
typedef int bool;
#define true 1
#define false 0
#endif

// #define LOG(...) printf(__VA_ARGS__);
#define LOG(...)

typedef struct Ctx {
    int *c;
    int *t;
    int sz;
} Ctx;

void initCtx(Ctx *ctx, int sz) {
    ctx->c = malloc(sizeof(int) * sz);
    ctx->t = malloc(sizeof(int) * sz);
    ctx->sz = sz;
    memset(ctx->c, 0, sizeof(int) * sz);
}

void deinitCtx(Ctx *ctx) {
    free(ctx->c);
    free(ctx->t);
}

bool pros(Ctx *ctx, int v) {
    int i;
    for (i = 0; i != ctx->sz; i++) {
        if (ctx->c[i] != 0 && ctx->t[i] == v) {
            LOG("pros %d at %d\n", v, i);
            ctx->c[i]++;
            return true;
        }
    }
    return false;
}

bool atnd(Ctx *ctx, int v) {
    int i;
    for (i = 0; i != ctx->sz; i++) {
        if (ctx->c[i] == 0) {
            ctx->c[i] = 1;
            ctx->t[i] = v;
            LOG("atnd %d at %d\n", v, i);
            return true;
        }
    }
    return false;
}

void cons(Ctx *ctx) {
    LOG("cons all\n");
    int i;
    for (i = 0; i != ctx->sz; i++) {
        if (ctx->c[i] != 0) {
            ctx->c[i]--;
        }
    }
}

int *retr(Ctx *ctx, int *sz) {
    int *r = malloc(sizeof(int) * ctx->sz);
    int i, c;
    for (i = 0, c = 0; i != ctx->sz; i++) {
        if (ctx->c[i] != 0) {
            r[c] = ctx->t[i];
            c++;
        }
    }
    *sz = c;
    return r;
}

void show(Ctx *ctx) {
    int i, start;
    printf("ctx: [");
    for (i = 0, start = 0; i != ctx->sz; i++) {
        if (ctx->c[i] != 0) {
            if (start == 0) {
                start = 1;
            } else {
                printf(", ");
            }
            printf("%d:%d", ctx->t[i], ctx->c[i]);
        }
    }
    printf("]\n");
}

void vrfy(int *nums, int numsSize, int thresh, int *cand, int *candSize) {
    int i, j, *c = malloc(sizeof(int) * (*candSize));
    memset(c, 0, sizeof(int) * (*candSize));
    for (i = 0; i != numsSize; i++) {
        for (j = 0; j != *candSize; j++) {
            if (nums[i] == cand[j]) {
                c[j]++;
            }
        }
    }
    for (j = 0, i = 0; j != *candSize; j++) {
        if (c[j] > thresh) {
            cand[i] = cand[j];
            i++;
        }
    }
    *candSize = i;
}

/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 *
 * Misra-Geries algorithm
 */
#define K   3

int *majorityElement(int *nums, int numsSize, int *returnSize) {
    Ctx ctx, *pctx = &ctx;;
    // at most K - 1 elements ocurr greater than [n / K] times.
    initCtx(pctx, K - 1);
    int i, j;
    for (i = 0; i != numsSize; i++) {
        if (pros(pctx, nums[i])) {
        } else if (atnd(pctx, nums[i])) {
        } else {
            cons(pctx);
        }
    }
    int *r = retr(pctx, returnSize);
    deinitCtx(pctx);

    // have to one more pass to verify it really greater than [n / K] times.
    vrfy(nums, numsSize, numsSize / K, r, returnSize);
    return r;
}

#define CASE(a, e) { \
    int n, *r = majorityElement(a, sizeof(a) / sizeof(int), &n); \
    int i, flag, l = sizeof(e) / sizeof(int); \
    if (n != l) { \
        flag = false; \
    } else { \
        for (i = 0, flag = true; i != n; i++) { \
            if (r[i] != e[i]) { flag = false; break; } \
        } \
    } \
    printf("%s\n", flag ? "PASS" : "fail"); }

int main() {
    CASE(((int[]) {1, 1, 1, 2, 2, 3, 3, 2, 2, 2, 3, 2, 2}), ((int[]) {2}));
    CASE(((int[]) {1, 1, 2, 3, 2}), ((int[]) {1, 2}));
    CASE(((int[]) {1, 1, 3}), ((int[]) {1}));
    CASE(((int[]) {1, 1, 2, 3, 4}), ((int[]) {1}));
    return 0;
}
