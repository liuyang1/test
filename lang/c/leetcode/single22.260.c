/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* singleNumber(int* nums, int numsSize, int* returnSize) {
    *returnSize = 2;
    int *r = malloc(sizeof(int) * (*returnSize));
    int i;
    for (i = 0; i != 2; i++) {
        r[i] = 0;
    }
    int s;
    for (i = 0, s = 0; i != numsSize; i++) {
        s ^= nums[i];
    }
    // Assume, two value just appear once is A, B.
    // so S = A ^ B;
    // find right most 1bit position.
    // For example, A = 0b111, B = 0b1001, S = 0b1110, diff = 0b10
    // so A & 0b10, B & 0b10, one is 1, other is 0.
    // Then sperate xor them.
    int diff = s & ~(s - 1);
    for (i = 0; i != numsSize; i++) {
        if (nums[i] & diff) {
            r[0] ^= nums[i];
        } else {
            r[1] ^= nums[i];
        }
    }
    return r;
}
