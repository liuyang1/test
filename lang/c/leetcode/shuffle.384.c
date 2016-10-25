#include "leet.h"
typedef struct {
    int *nums;
    int size;
} Solution;

Solution *solutionCreate(int *nums, int size) {
    Solution *sln = malloc(sizeof(*sln));
    sln->nums = nums;
    sln->size = size;
    return sln;
}

/** Resets the array to its original configuration and return it. */
int *solutionReset(Solution *sln, int *returnSize) {
    int *ret = malloc(sizeof(*ret) * sln->size);
    memcpy(ret, sln->nums, sizeof(*ret) * sln->size);
    *returnSize = sln->size;
    return ret;
}

/** Returns a random shuffling of the array. */
int *solutionShuffle(Solution *sln, int *returnSize) {
    int *ret = solutionReset(sln, returnSize);
    if (sln->size == 0) {
        return ret;
    }

    int i, t;
    for (i = sln->size - 1; i != 0; i--) {
        int p = rand() % (i + 1);
        if (p == i) {
            continue;
        }
        t = ret[p];
        ret[p] = ret[i];
        ret[i] = t;
    }

    return ret;
}

void solutionFree(Solution *sln) {
    free(sln);
}

// test code
void doShuffle(int *nums, int len) {
    Solution *sln = solutionCreate(nums, len);
    int olen;
    int *o = solutionReset(sln, &olen);
    showArr(o, olen);
    free(o);
    int i;
    for (i = 0; i != 1; i++) {
        int rlen;
        int *r = solutionShuffle(sln, &rlen);
        showArr(r, rlen);
        free(r);
    }
    solutionFree(sln);
}

void try() {
    const int len = 0;
    int *a = malloc(sizeof(int) * len);
    int i;
    for (i = 0; i != len; i++) {
        a[i] = i;
    }
    doShuffle(a, len);
    free(a);
}

void tryCase() {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
               24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
               45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
               66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
               87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, -11, -99, -98, -97, -96, -95,
               -94, -93, -92, -91, -90, -89, -88, -87, -86, -85, -84, -83, -82, -81, -80, -79, -78,
               -77, -76, -75, -74, -73, -72, -71, -70, -69, -68, -67, -66, -65, -64, -63, -62, -61,
               -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44,
               -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27,
               -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -2, -10,
               -9, -8, -7, -6, -5, -4, -3, -1};
    // I don't know why OJ always return Runtime Error on this case.
    doShuffle(a, COUNT_OF(a));
}

int main() {
    try();
    tryCase();
    return 0;
}
