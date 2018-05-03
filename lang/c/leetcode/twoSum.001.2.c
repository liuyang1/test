#include <stdlib.h>
#include <stdio.h>
/**
 * Note: The returned array must be malloced, assume caller calls free()
 *
 * Solution:
 * - sort array
 * - search from bottom and top same time, try to sum
 *      if less, increase bottom
 *      if greater, decrease top
 *      if equal, output final result
 *
 * Complexity: O(n * log(n))
 * Actually, sort step is more complex than sum-step
 */

typedef struct {
    int value;
    int index;
} Node;

int compare_Node(const void *n0, const void *n1)
{
    return ((Node *)(n0))->value > ((Node *)(n1))->value;
}

int* twoSum(int* nums, int numsSize, int target) {
    int i;
    Node *array = (Node *)malloc(sizeof(Node) * numsSize);
    for (i = 0; i != numsSize; i++) {
        array[i].value = nums[i];
        array[i].index = i;
    }
    qsort(array, numsSize, sizeof(Node), compare_Node);

    int *ret = (int *)malloc(sizeof(int) * 2);
    int bot = 0, top = numsSize - 1;
    int sum;
    while (1) {
        sum = array[bot].value + array[top].value;
        if (sum < target) {
            bot++;
        } else if(sum > target) {
            top--;
        } else {
            top = array[top].index + 1;
            bot = array[bot].index + 1;
            if (top < bot) {
                ret[0] = top;
                ret[1] = bot;
            } else {
                ret[0] = bot;
                ret[1] = top;
            }
            break;
        }
        if (bot >= top) {
            // should not here
            free(ret);
            ret = NULL;
            break;
        }
    }
    free(array);
    return ret;
}

int main()
{
    int nums[] = {0, 3, 4, 0};
    int *ret = twoSum(nums, sizeof(nums) / sizeof(int), 0);
    if (ret != NULL) {
        if (ret[0] == 1 && ret[1] == 4) {
            printf("PASS\n");
        } else {
            printf("FAIL ret: %d %d\n", ret[0], ret[1]);
        }
        free(ret);
    } else {
        printf("FAIL ret NULL\n");
    }
    return 0;
}
