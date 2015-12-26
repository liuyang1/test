#include <stdio.h>

void moveZeroes(int* nums, int numsSize) {
    int i, b;
    for (i = 0, b = 0; i != numsSize; i++) {
        if (nums[i] == 0) {
            //
        } else {
            nums[b] = nums[i];
            b++;
        }
    }
    for (; b != numsSize; b++) {
        nums[b] = 0;
    }
    showArr(nums, numsSize);
}

void showArr(int *nums, int numsSize) {
    int i;
    printf("[");
    for (i = 0; i != numsSize; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", nums[i]);
    }
    printf("]\n");
}

#define CASE(a) {moveZeroes(a, sizeof(a) / sizeof(int));}
int main()
{
    CASE(((int []){0, 1, 2}));
    CASE(((int []){0, 1, 3, 0, 12}));
    CASE(((int []){}));
    return 0;
}
