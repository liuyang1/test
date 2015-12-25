#include <stdlib.h>
#include <stdio.h>

#define INT_STR_LEN     11
char **summaryRanges(int *nums, int numsSize, int *returnSize)
{
    if (numsSize == 0) {
        return NULL;
    }
    char **retArr = malloc(numsSize * sizeof(char *));
    int i, j, len, strsz;
    for (i = 0, j = 0; i != numsSize; i += len + 1, j++) {
        for (len = 0; i + len + 1 < numsSize && nums[i + len] + 1 == nums[i + len + 1]; len++) {
        }
        if (len == 0) {
            strsz = INT_STR_LEN + 1;
            retArr[j] = malloc(strsz);
            strsz = snprintf(retArr[j], strsz, "%d", nums[i]);
        } else {
            strsz = INT_STR_LEN * 2 + 2 + 1;
            retArr[j] = malloc(strsz);
            strsz = snprintf(retArr[j], strsz,  "%d->%d", nums[i], nums[i + len]);
        }
    }
    *returnSize = j;
    return retArr;
}

#define CASE(a) { \
        int r, i; \
        char **p = summaryRanges(a, sizeof(a) / sizeof(int), &r); \
        if (p == NULL) { \
            printf("%p\n", p); \
        } else { \
            for (i = 0; i < r; i++) { \
                if (i != 0) { \
                    printf(", "); \
                } \
                printf("\"%s\"", p[i]); \
                free(p[i]); \
            } \
            printf("\n"); \
            free(p); \
        } \
}
int main()
{
    CASE(((int[]) {1, 2, 3}));
    CASE(((int[]) {1, 3, 5}));
    CASE(((int[]) {0, 1, 2, 4, 5, 7}));
    CASE(((int[]) {0}));
    CASE(((int[]) {1}));
    CASE(((int[]) {0x7fffffff}));
    CASE(((int[]) {-0x7fffffff}));
    CASE(((int[]) {-1}));
    CASE(((int[]) {}));
    return 0;
}
