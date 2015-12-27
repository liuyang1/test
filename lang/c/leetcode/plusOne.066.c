/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    if (digits == NULL || digitsSize <= 0) {
        *returnSize = 0;
        return NULL;
    }
    int *ret = malloc(sizeof(int) * (digitsSize + 1));
    int i, j, carry, s;
    for (i = digitsSize - 1, j = digitsSize, carry = 1;
         i >= 0;
         i--, j--) {
        s = carry + digits[i];
        ret[j] = s % 10;
        carry = s / 10;
    }
    if (carry != 0) {
        ret[0] = carry;
        *returnSize = digitsSize + 1;
        return ret;
    }
    for (i = 0; i != digitsSize; i++) {
        ret[i] = ret[i + 1];
    }
    *returnSize = digitsSize;
    return ret;
    
}
