#include <stdio.h>
void showArray(int *a, int n)
{
    int i;
    printf("[");
    for (i = 0; i != n; i++) {
        if (i != 0) {
            putchar(',');
        }
        printf(" %d", a[i]);
    }
    printf("]\n");
}

void rotateI(int* nums, int k) {
    int i, swap;
    int thresh = k % 2 == 0 ? k / 2 : k / 2 + 1;
    for (i = 0; i != thresh; i++) {
        swap = nums[i];
        nums[i] = nums[k - i - 1];
        nums[k - i - 1] = swap;
    }
}

void rotate(int* nums, int numsSize, int k) {
    k = k % numsSize;
    if (k == 0) {
        return;
    }
    rotateI(nums, numsSize - k);
    rotateI(nums + numsSize - k, k);
    rotateI(nums, numsSize);
}

void case0()
{
    int a[] = {1};
    showArray(a, 1);
    rotate(a, 1, 0);
    showArray(a, 1);
}
void case1()
{
    int a[] = {1, 2, 3, 4, 5};
    showArray(a, 5);
    rotate(a, 5, 6);
    showArray(a, 5);
}

int main()
{
    case0();
    case1();
    return 0;
}
