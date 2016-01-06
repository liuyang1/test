#include "leet.h"
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int *a, int n) {
    int i, t;
    for (i = 0; i != n / 2; i++) {
        t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}

typedef struct {
    int *st;
    int n;
    int cnt;
} Ctx;

void initCtx(Ctx *p, int n) {
    p->n = n;
    p->st = malloc(sizeof(int) * n);
    p->cnt = 0;
}
void finitCtx(Ctx *p) {
    free(p->st);
}

bool next(Ctx *p) {
    int i, j;
    if (p->cnt == 0) {
        p->cnt++;
        for (i = 0; i != p->n; i++) {
            p->st[i] = i;
        }
        return true;
    }
    for (i = p->n - 1; i > 0; i--) {
        if (p->st[i] > p->st[i - 1]) {
            break;
        }
    }
    for (j = p->n - 1; j > 0; j--) {
        if (p->st[j] > p->st[i - 1]) {
            break;
        }
    }
    if (i == 0) {
        return false;
    }
    swap(p->st + i - 1, p->st + j);
    reverse(p->st + i, p->n - i);

    p->cnt++;
    return true;
}

void snapshot(Ctx *p, int *save) {
    int i;
    for (i = 0; i != p->n; i++) {
        save[i] = p->st[i];
    }
}
int** permute(int* nums, int numsSize, int* returnSize) {
    int perm = 1;
    int i, j;
    for (i = numsSize; i != 1; i--) {
        perm *= i;
    }

    Ctx ctx, *p = &ctx;
    initCtx(p, numsSize);

    int **r = malloc(sizeof(int *) * perm);
    for (i = 0; i != perm; i++) {
        next(p);
        r[i] = malloc(sizeof(int) * numsSize);
        for (j = 0; j != numsSize; j++) {
            r[i][j] = nums[p->st[j]];
        }
    }

    finitCtx(p);

    *returnSize = perm;
    return r;
}

int testCtx(int n) {
    Ctx ctx, *p = &ctx;
    initCtx(p, n);
    int cnt = 0;
    while(next(p)) {
        cnt++;
        printf("%d:\t", p->cnt);
        showArr(p->st, n);
    }
    finitCtx(p);
    return 0;
}

int testPerm(int *nums, int sz) {
    int retsz;
    int **r = permute(nums, sz, &retsz);
    int i;
    for (i = 0; i != retsz; i++) {
        showArr(r[i], sz);
    }
    return 0;
}

int main() {
    // testCtx(4);
    int nums[] = {1, 2, 3};
    testPerm(nums, 3);
    int nums1[] = {4, 5, 6, 0};
    testPerm(nums1, 4);
    return 0;
}
