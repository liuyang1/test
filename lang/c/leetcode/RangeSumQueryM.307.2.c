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
    int *array;
    int *tree;
    int size;
};

void show(struct NumArray *numArray) {
    printf("size=%d\t", numArray->size);
    showArr(numArray->array, numArray->size);
}

void addOne(struct NumArray *narray, int i, int delta) {
    LOG("enter i=%d delta=%d\n", i, delta);
    if (delta == 0) {
        return;
    }
    while (i < narray->size) {
        narray->tree[i] += delta;
        i += i & (-i);
    }
}

struct NumArray *NumArrayCreate(int *nums, int numsSize) {
    struct NumArray *r = malloc(sizeof(struct NumArray));
    r->size = numsSize;
    if (r->size == 0) {
        r->array = NULL;
        return r;
    }
    r->array = malloc(sizeof(int) * r->size);
    r->tree = malloc(sizeof(int) * r->size);
    memcpy(r->array, nums, sizeof(int) * r->size);
    memset(r->tree, 0, sizeof(int) * r->size);
    LOG("%s", "test\n");
    int i;
    for (i = 0; i != r->size; i++) {
        addOne(r, i, nums[i]);
    }
    show(r);
    return r;
}

void update(struct NumArray *narray, int i, int val) {
    if (i < 0 || i >= narray->size) {
        return;
    }
    int delta = val - narray->array[i];
    addOne(narray, i, delta);
}

int sumRange(struct NumArray *narray, int i, int j) {
    if (i > 0) {
        return sumRange(narray, 0, j) - sumRange(narray, 0, i - 1);
    } else if (i < 0) {
        i = 0;
    }
    if (j < 0) {
        return 0;
    } else if (j > narray->size - 1) {
        j = narray->size - 1;
    }
    if (narray->size == 0) {
        return 0;
    }
    int sum = 0;
    while (j < narray->size) {
        sum += narray->tree[i];
        i -= i & (-i);
    }
    return sum;

}

void NumArrayFree(struct NumArray *numArray) {
    if (numArray->tree) {
        free(numArray->tree);
    }
    if (numArray->array) {
        free(numArray->array);
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

bool case0(int *nums, int numsSize) {
    struct NumArray *narray = NumArrayCreate(nums, numsSize);
    show(narray);
    update(narray, 3, 5);
    show(narray);
    NumArrayFree(narray);
    return true;
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
    // case1();
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    case0(a, COUNT_OF(a));
    return 0;
}
