/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
int maxDepth(struct TreeNode* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL) {
        return maxDepth(root->right) + 1;
    }
    if (root->right == NULL) {
        return maxDepth(root->left) + 1;
    }
    int lm = maxDepth(root->left);
    int rm = maxDepth(root->right);
    return (lm > rm ? lm : rm) + 1;
}