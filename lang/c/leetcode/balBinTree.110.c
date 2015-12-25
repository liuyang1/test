/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
int depthBal(struct TreeNode* root) {
    if (root == NULL) {
        return 0;
    }
    int d0 = depthBal(root->left);
    int d1 = depthBal(root->right);
    if (d0 < 0 || d1 < 0) {
        return -1;
    }
    if (d0 == d1 || d0 == d1 + 1 || d0 + 1 == d1) {
        return (d0 > d1 ? d0 : d1) + 1;
    }
    return -1;
}
bool isBalanced(struct TreeNode* root) {
    return -1 != depthBal(root);
}