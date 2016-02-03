#include "leet.h"
int perm(int n) {
    if (n <= 1) {
        return 1;
    }
    int i, p;
    for (i = 2, p = 1; i <= n; i++) {
        p *= i;
    }
    return p;
}

typedef struct {
    int *nums;
    int size;
    int nth;
    int mod;
} Context;

Context *ctx_create(int n, int k) {
    Context *ctx = malloc(sizeof(Context));
    ctx->size = n;
    ctx->nth = k;
    ctx->mod = perm(ctx->size - 1);
    ctx->nums = malloc(sizeof(int) * ctx->size);
    int i;
    for (i = 0; i != n; i++) {
        ctx->nums[i] = i + 1;
    }
    return ctx;
}
void ctx_show(Context *ctx) {
    printf("ctx nth=%d size=%d mod=%d\t", ctx->nth, ctx->size, ctx->mod);
    showArr(ctx->nums, ctx->size);
}
bool ctx_end(Context *ctx) {
    return ctx->size <= 0;
}
int ctx_permseq(Context *ctx) {
    int i = ctx->nth / ctx->mod;
    int r = ctx->nums[i];
    int j;
    for (j = i; j != ctx->size; j++) {
        ctx->nums[j] = ctx->nums[j + 1];
    }
    ctx->nth -= i * ctx->mod;
    ctx->size--;
    if (ctx->size != 0) {
        ctx->mod /= ctx->size;
    }
    return r;
}
void ctx_destory(Context *ctx) {
    free(ctx->nums);
    free(ctx);
}

int digitLen(int n) {
    if (n == 0) {
        return 1;
    }
    if (n < 0) {
        return 1 + digitLen(-1 * n);
    }
    int len = 0;
    while (n != 0) {
        n /= 10;
        len++;
    }
    return len;
}
int guessSize(int n) {
    return (n + 1) * digitLen(n) + 1;
}
char* getPermutation(int n, int k) {
    int len = guessSize(n);
    char *r = malloc(sizeof(char) * len);
    int offset = 0;

    k--; // offset-by-one
    Context *ctx = ctx_create(n, k);
    int ret;
    while (!ctx_end(ctx)) {
        ret = ctx_permseq(ctx);
        offset += snprintf(r + offset, len - offset - 1, "%d", ret);
    }
    ctx_destory(ctx);

    return r;
}

// test code
bool allisdigit(char *r) {
    if (r == NULL) {
        return false;
    }
    for (;*r != '\0'; r++) {
        if (!isdigit(*r)) {
            return false;
        }
    }
    return true;
}
bool chkresult(char *last, char *ret) {
    return allisdigit(ret) && (last == NULL || strcmp(last, ret) < 0);
}
bool fulltest(int n) {
    int i, thresh = perm(n);
    char *last = NULL, *ret;
    for (i = 0; i != thresh; i++) {
        ret = getPermutation(n, i + 1);
        printf("%s ", ret);
        if (!chkresult(last, ret)) {
            return false;
        }
        if (last != NULL) {
            free(last);
        }
        last = ret;
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    if (i % 10 != 0) {
        printf("\n");
    }
    if (last) {
        free(last);
    }
    return true;
}
bool nfulltest(int n) {
    bool r;
    int i;
    for (i = 1; i <= n; i++) {
        r = fulltest(i);
        printf("fulltee(%d) %s\n", i, expect(r));
        if (!r) {
            return false;
        }
    }
    return true;
}
bool simpletest(int n, int k) {
    char *ret = getPermutation(n, k);
    bool r = allisdigit(ret);
    printf("permSeq(%d, %d) = %s %s\n", n, k, ret, expect(r));
    return r;
}
int main() {
    nfulltest(7);
    simpletest(10, 1000);
    return 0;
}
