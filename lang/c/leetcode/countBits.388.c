#include "leet.h"

#define INT_LEN     32
int *countBits(int num, int *returnSize) {
    *returnSize = num + 1;
    int *r = malloc(sizeof(int) * (*returnSize));
    int bit[INT_LEN] = {0};
    int i, j, sum = 0, carry;
    r[0] = 0;
    // How complexity of this loop?
    // This is interesting problem.
    //      sum n/2^n, n=1 to infinity => 2
    // https://www.wolframalpha.com/input/?i=sum+n%2F2%5En,+n%3D1+to+infinity
    // So this complexity is O(2n)
    for (i = 1; i != num + 1; i++) {
        for (j = 0, carry = 1; j != INT_LEN; j++) {
            bit[j] += carry;
            if (bit[j] == 2) {
                carry = 1;
                bit[j] = 0;
                sum -= 1;
            } else {
                sum += 1;
                break;
            }
        }
        r[i] = sum;
    }
    return r;
}

int *countBits1(int num, int *returnSize) {
    int *r = malloc(sizeof(int) * (*returnSize));
    int i, j;
    // smart solution, O(n)
    // 0
    // 0 1
    // 0 1 1 2
    // 0 1 1 2 1 2 2 3
    // 0 1 1 2 1 2 2 3 1 2 2 3 2 3 3 4
    // copy last region, and increment every one
    r[0] = 0;
    for (i = 1; i <= num; i *= 2) {
        for (j = 0; j != i; j++) {
            if (i + j > num) {
                goto final;
            }
            r[i + j] = r[j] + 1;
        }
    }
final:
    *returnSize = num + 1;
    return r;
}

void testCount(int n, int *(countFunc)(int, int *)) {
    int sz;
    int *r = countFunc(n, &sz);
    showArr(r, sz);
    free(r);
}

int main() {
    int n = 100;
    testCount(n, countBits);
    testCount(n, countBits1);
    return 0;
}
