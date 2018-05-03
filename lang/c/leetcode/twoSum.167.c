/**
 * same solution with twoSum.001.2.c
 * but input data is sorted
 */

#include "leet.h"
int *twoSum(int *n, int sz, int target, int *retsz) {
    int *ret = (int *)malloc(sizeof(int) * 2);
    int b, e, sum;
    for (b = 0, e = sz - 1; b < e;) {
        sum = n[b] + n[e];
        if (sum < target) {
            b++;
        } else if (sum > target) {
            e--;
        } else {
            // find one solution, return one-based index
            ret[0] = b + 1;
            ret[1] = e + 1;
            break;
        }
    }
    if (b >= e) {
        free(ret);
        *retsz = 0;
        return NULL;
    } else {
        *retsz = 2;
        return ret;
    }
}

bool unit(int *n, int sz, int target, int *e, int esz) {
    int len;
    int *ret = twoSum(n, sz, target, &len);
    bool r = cmpArr(ret, len, e, esz);
    if (!r) {
        showArr(n, sz);
        printf("twoSum(arr) = ");
        showArr(ret, len);
        printf(" != ");
        showArr(e, esz);
        printf(" %s\n", expect(r));
        return r;
    }
    return r;
}

int main() {
    int n0[] = {0, 0, 3, 4};
    int e0[] = {1, 2};
    unit(n0, COUNT_OF(n0), 0, e0, COUNT_OF(e0));

    int n1[] = {2, 7, 11, 15};
    int e1[] = {1, 2};
    unit(n1, COUNT_OF(n1), 9, e1, COUNT_OF(e1));

    return 0;
}
