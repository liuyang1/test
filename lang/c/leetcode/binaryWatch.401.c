#include "leet.h"

int combination(int n, int k) {
    if (n - k < k) {
        k = n - k;
    }
    int i, j;
    int r = 1;
    for (i = n, j = 1; j != k + 1; i--, j++) {
        r = (i * r) / j;
    }
    return r;
}

typedef struct {
    int *st;
    int n;
    int k;
    int cnt;
} Ctx;

void initCtx(Ctx *ctx, int n, int k) {
    ctx->n = n;
    ctx->k = k;
    ctx->cnt = 0;
    ctx->st = malloc(sizeof(*ctx->st) * k);
}

bool next(Ctx *ctx) {
    int i;
    ctx->cnt++;
    if (ctx->cnt == 1) {
        for (i = 0; i != ctx->k; i++) {
            ctx->st[i] = i;
        }
        return true;
    }
    for (i = ctx->k - 1; i >= 0; i--) {
        ctx->st[i]++;
        if (ctx->st[i] - i + (ctx->k - 1) != ctx->n) {
            break;
        }
    }
    if (i < 0) {
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

// from BITS select NUM 1s, then combine to number
int buildFromBits(int *bits, int len) {
    int r, i;
    for (r = i = 0; i != len; i++) {
        r += 1 << bits[i];
    }
    return r;
}

int *readBinary(int bits, int num, int *returnSize) {
    int n = combination(bits, num);
    int *ret = malloc(sizeof(int) * n);

    Ctx ctx, *p = &ctx;
    initCtx(p, bits, num);
    int *st = malloc(sizeof(int) * num);

    int i;
    for (i = 0; next(p); i++) {
        snapshot(p, st);
        ret[i] = buildFromBits(st, num);
    }

    *returnSize = n;
    free(st);
    finitCtx(p);
    return ret;
}

#define HOURMAX 4
#define MINMAX  6
#define TIMELEN 6
char **readBinaryWatch(int num, int *returnSize) {
    int n;
    int h, m;
    for (h = n = 0; h != num + 1; h++) {
        m = num - h;
        if (h > HOURMAX || m > MINMAX) {
            continue;
        }
        n += combination(HOURMAX, h) * combination(MINMAX, m);
    }
    char **ret = malloc(sizeof(char *) * n);

    for (h = n = 0, m = num; h != num + 1; h++, m--) {
        if (h > HOURMAX || m > MINMAX) {
            continue;
        }
        int hn, mn;
        int *hs = readBinary(HOURMAX, h, &hn);
        int *ms = readBinary(MINMAX, m, &mn);
        int j, k;
        for (j = 0; j != hn; j++) {
            for (k = 0; k != mn; k++) {
                if (hs[j] < 12 && ms[k] < 60) {
                    ret[n] = malloc(sizeof(char) * TIMELEN);
                    sprintf(ret[n], "%d:%02d", hs[j], ms[k]);
                    n++;
                }
            }
        }
        free(hs);
        free(ms);
    }
    *returnSize = n;
    return ret;
}

int testReadBinary() {
    int len;
    int *r = readBinary(4, 3, &len);
    showArr(r, len);
    free(r);
    return 0;
}

int testReadBinaryWatch(int n) {
    int len;
    char **r = readBinaryWatch(n, &len);
    showStr2(r, len);
    int i;
    for (i = 0; i != len; i++) {
        free(r[i]);
    }
    free(r);
    return 0;
}

int main() {
    testReadBinaryWatch(0);
    testReadBinaryWatch(1);
    testReadBinaryWatch(2);
    testReadBinaryWatch(3);
    testReadBinaryWatch(4);
    return 0;
}
