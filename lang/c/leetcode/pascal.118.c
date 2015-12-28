#include <stdio.h>
/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *columnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** generate(int numRows, int** columnSizes, int* returnSize) {
    if (numRows <= 0) {
        *columnSizes = NULL;
        *returnSize = 0;
        return NULL;
    }
    int **ret = malloc(sizeof(int *) * numRows);
    *columnSizes = malloc(sizeof(int) * numRows);
    *returnSize = numRows;
    int i, j;
    for (i = 0; i != numRows; i++) {
        (*columnSizes)[i] = i + 1;
        ret[i] = malloc(sizeof(int) * (i + 1));
    }
    ret[0][0] = 1;
    for (i = 1; i != numRows; i++) {
        ret[i][0] = 1;
        for (j = 1; j != i; j++) {
            ret[i][j] = ret[i - 1][j - 1] + ret[i - 1][j];
        }
        ret[i][j] = 1;
    }
    return ret;
}

void showArrArr(int **ret, int *columnSizes, int returnSize) {
    int i, j;
    printf("[");
    for (i = 0; i != returnSize; i++) {
        printf("[");
        for (j = 0; j != columnSizes[i]; j++) {
            if (j != 0) {
                printf(", ");
            }
            printf("%d", ret[i][j]);
        }
        if (i != returnSize - 1) {
            printf("], ");
        } else {
            printf("]");
        }

    }
    printf("]\n");
}
int main() {
    int *columnSizes;
    int returnSize;
    int **ret = generate(5, &columnSizes, &returnSize);
    showArrArr(ret, columnSizes, returnSize);
    return 0;
}
