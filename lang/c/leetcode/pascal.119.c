/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* getRow(int rowIndex, int* returnSize) {
    if (rowIndex < 0) {
        return NULL;
    }
    *returnSize = rowIndex + 1;
    int *ret = malloc(sizeof(int) * (rowIndex + 1));
    int i, j;
    for (i = 0, ret[0] = 1; i <= rowIndex; i++) {
        ret[i] = 1;
        for (j = i - 1; j > 0; j--) {
            ret[j] += ret[j - 1];
        }
        ret[0] = 1;
    }
    return ret;
}
