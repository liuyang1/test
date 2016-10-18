#include "leet.h"
enum {
    left = 0,
    right = 1,
};
int sumOfLeftLeavesIn(struct TreeNode *root, int from, int acc) {
    if (root->left == NULL && root->right == NULL) {
        if (from == left) {
            acc += root->val;
        }
    }
    if (root->left != NULL) {
        acc += sumOfLeftLeavesIn(root->left, left, 0);
    }
    if (root->right != NULL) {
        acc += sumOfLeftLeavesIn(root->right, right, 0);
    }
    return acc;
}

int sumOfLeftLeaves(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return sumOfLeftLeavesIn(root, right, 0);
}

int test() {
    int a[] = {3, 9, 20, NIL, NIL, 15, 7};
    struct TreeNode *t = buildTree(a, COUNT_OF(a));
    showTree(t);
    int v = sumOfLeftLeaves(t);
    printf("%d\n", v);
    return 0;
}

int main() {
    test();
    return 0;
}
