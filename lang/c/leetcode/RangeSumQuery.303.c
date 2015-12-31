struct NumArray {
    int *acc;
    int len;
};

/** Initialize your data structure here. */
struct NumArray* NumArrayCreate(int* nums, int numsSize) {
    struct NumArray *numArray = malloc(sizeof(struct NumArray));
    numArray->len = numsSize;
    if (numsSize <= 0) {
        numArray->acc = NULL;
        return numArray;
    }
    numArray->acc = (int *)malloc(sizeof(int) * numsSize);
    int i;
    for (i = 1, numArray->acc[0] = nums[0]; i != numsSize; i++) {
        numArray->acc[i] = numArray->acc[i - 1] + nums[i];
    }
    return numArray;
}

int sumRange(struct NumArray* numArray, int i, int j) {
    if (i > j || i < 0 || j > numArray->len) {
        // Out of range exception
        return 0;
    }
    if (i == 0) {
        return numArray->acc[j];
    } else {
        return numArray->acc[j] - numArray->acc[i - 1];
    }
}

/** Deallocates memory previously allocated for the data structure. */
void NumArrayFree(struct NumArray* numArray) {
    if(numArray->acc != NULL) {
        free(numArray->acc);
    }
}

// Your NumArray object will be instantiated and called as such:
// struct NumArray* numArray = NumArrayCreate(nums, numsSize);
// sumRange(numArray, 0, 1);
// sumRange(numArray, 1, 2);
// NumArrayFree(numArray);
