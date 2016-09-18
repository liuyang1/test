#include "leet.h"

int countNodes(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return countNodes(root->left) + countNodes(root->right);
}
