#include <stdio.h>

int removeElement(int* nums, int numsSize, int val) {
    int i, b;
    for (i = 0, b = 0; i != numsSize; i++) {
        if (nums[i] == val) {
        } else {
            nums[b] = nums[i];
            b++;
        }
    }
    showArr(nums, b);
    return b;
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

#define CASE(a, v) {removeElement(a, sizeof(a) / sizeof(int), v);}
int main()
{
    CASE(((int []){0, 1, 2}), 0);
    CASE(((int []){0, 1, 2}), 1);
    CASE(((int []){0, 1, 2}), 2);
    CASE(((int []){0, 1, 2}), 4);
    CASE(((int []){0, 1, 3, 0, 12}), 0);
    CASE(((int []){}), 1);
    return 0;
}
