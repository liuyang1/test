// max value must >= 2 * ABS(MIN_INT), this greater than MAX_UINT, so use
// `unsigned long long`
#define BASE 3
unsigned long long xor3(unsigned long long a, unsigned long long b) {
    unsigned int r = 0;
    unsigned int weight = 1;
    while (a != 0 || b != 0) {
        r += ((a + b) % BASE) * weight;
        weight *= BASE;
        a /= BASE;
        b /= BASE;

    }
    return r;
}

int singleNumber(int* nums, int numsSize) {
    unsigned long long r, i;
    int signBit = 0;
    for (i = 0, r = 0; i != numsSize; i++) {
        if (nums[i] < 0) {
            signBit = (signBit + 1) % 3;
            r = xor3(r, nums[i] * (-1ll));
        } else {
            r = xor3(r, nums[i]);
        }
    }
    return signBit == 0 ? r : -r;
}

#include <stdio.h>
#define CASE(a, e) {int r = singleNumber(a, sizeof(a) / sizeof(int)); printf("ret=%d %s\n", r, r == e ? "PASS" : "fail"); }
int main() {
    CASE(((int []){1}), 1);
    CASE(((int []){1, 1, 1, 100}), 100);
    CASE(((int []){1, 1, 1, 100, 100, 100, 30}), 30);
    CASE(((int []){1, 1, 1, 100, 100, 100, -30}), -30);
    CASE(((int []){1, 1, 1, -100, -100, -100, -30}), -30);
    CASE(((int []){43,16,45,89,45,-2147483648,45,2147483646,-2147483647,-2147483648,43,2147483647,-2147483646,-2147483648,89,-2147483646,89,-2147483646,-2147483647,2147483646,-2147483647,16,16,2147483646,43}), 2147483647);
    return 0;
}
