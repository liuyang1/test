/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
int minDepth(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL) {
        return minDepth(root->right) + 1;
    }
    if (root->right == NULL) {
        return minDepth(root->left) + 1;
    }
    int lm = minDepth(root->left);
    int rm = minDepth(root->right);
    return (lm < rm ? lm : rm) + 1;
}
