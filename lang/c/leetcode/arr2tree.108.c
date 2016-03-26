#include "leet.h"
struct TreeNode *buildBSTree(int *nums, int numsSize) {
    if (nums == NULL || numsSize <= 0) {
        return NULL;
    }
    int idx = numsSize / 2; // prefer this one, left compact
    // int idx = (numsSize - 1) / 2; // to match leetcode's result
    struct TreeNode *n = malloc(sizeof(struct TreeNode));
    n->val = nums[idx];
    n->left = buildBSTree(nums, idx);
    n->right = buildBSTree(nums + idx + 1, numsSize - idx - 1);
    return n;
}

struct TreeNode *sortedArrayToBST(int *nums, int numsSize) {
    return buildBSTree(nums, numsSize);
}

#define CASE(a) {struct TreeNode *r = sortedArrayToBST(a, COUNT_OF(a)); \
                 showTree(r); }
int main() {
    CASE(((int[]) {1, 2, 3, 4, 5}));
    CASE(((int[]) {1, 2, 3, 4, 5, 6, 7, 8}));
    return 0;
}
