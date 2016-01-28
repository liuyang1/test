/*
 * RangeSumQuery mutable
 * assume the number of calls to UPDATE and SUMRANGE is distributed evenly.
 *
 * Array Tree struct.
 * create is O(n)
 * update, sumRange is O(n * log(n))
 */
#include "leet.h"
struct NumArray {
    int *tree;
    int numsSize;
    int treeSize;
};

int roundup(int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

void show(struct NumArray *numArray) {
    showArr(numArray->tree, numArray->treeSize);
    printf("numsSize=%d treeSize=%d tree=%p\n",
           numArray->numsSize, numArray->treeSize, numArray->tree);
}

struct NumArray *NumArrayCreate(int *nums, int numsSize) {
    struct NumArray *r = malloc(sizeof(struct NumArray));
    r->numsSize = numsSize;
    if (r->numsSize == 0) {
        r->tree = NULL;
        return r;
    }
    r->treeSize = roundup(numsSize) * 2;
    r->tree = malloc(sizeof(int) * r->treeSize);
    memset(r->tree, 0, sizeof(int) * r->treeSize);
    memcpy(r->tree + r->treeSize / 2, nums, sizeof(int) * numsSize);
    int level, i, bgn, end;
    for (level = r->treeSize / 2; level != 1; level /= 2) {
        bgn = level;
        end = 2 * level;
        for (i = bgn; i != end; i++) {
            r->tree[i / 2] += r->tree[i];
        }
    }
    return r;
}

void update(struct NumArray *numArray, int i, int val) {
    if (i < 0 || i >= numArray->numsSize) {
        return;
    }
    int idx = numArray->treeSize / 2 + i;
    int diff = val - numArray->tree[idx];
    for (; idx != 0; idx /= 2) {
        numArray->tree[idx] += diff;
    }
}

int sumRange(struct NumArray *numArray, int i, int j) {
    if (i > 0) {
        return sumRange(numArray, 0, j) - sumRange(numArray, 0, i - 1);
    } else if (i < 0) {
        i = 0;
    }
    if (j < 0) {
        return 0;
    } else if (j > numArray->numsSize - 1) {
        j = numArray->numsSize - 1;
    }
    if (numArray->numsSize == 0) {
        return 0;
    }
    int idx, rgn, s;
    for (s = 0; j >= 0; j -= rgn) {
        for (idx = j + numArray->treeSize / 2, rgn = 1;
             idx % 2 == 1;
             idx /= 2) {
            rgn *= 2;
        }
        if (idx == 0) {
            idx = 1;
        }
        s += numArray->tree[idx];
    }
    return s;

}

void NumArrayFree(struct NumArray *numArray) {
    if (numArray->tree) {
        free(numArray->tree);
    }
    free(numArray);
}

// test code
int simpleAcc(int *nums, int size, int i, int j) {
    int idx, s = 0;
    if (i < 0) {
        i = 0;
    }
    if (j > size - 1) {
        j = size - 1;
    }
    if (j < 0) {
        return 0;
    }
    LOG("%d %d\n", i, j);
    for (idx = i; idx != j + 1; idx++) {
        s += nums[idx];
    }
    return s;
}

bool testCase(int *nums, int numsSize) {
    struct NumArray *narray = NumArrayCreate(nums, numsSize);
    int s, i, j, e;
    int r = true;
    for (i = -1; i != numsSize + 1; i++) {
        for (j = i; j != numsSize + 1; j++) {
            s = sumRange(narray, i, j);
            e = simpleAcc(nums, numsSize, i, j);
            printf("[%d, %d] %d ?= %d %s\n", i, j, s, e, expect(s == e));
            if (s != e) {
                r = false;
                goto final;
            }
        }
    }
final:
    NumArrayFree(narray);
    return r;
}

#define CASE(a) {int r = testCase(a, COUNT_OF(a)); \
                 showArr(a, COUNT_OF(a)); printf("%s\n", expect(r)); }
int case1() {
    CASE(((int[]) {1, 3, 5, 3, 6, 4, 9, 2, 11}));
    CASE(((int[]) {1, 3, 5, 3, 6, 4, 9}));
    CASE(((int[]) {1, 2, 3}));
    CASE(((int[]) {1, 2}));
    CASE(((int[]) {1}));
    CASE(((int[]) {}));
    return 0;
}

int main() {
    case1();
    return 0;
}
