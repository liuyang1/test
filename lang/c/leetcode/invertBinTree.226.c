/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
struct TreeNode* invertTree(struct TreeNode* root) {
    if (root == NULL) {
        return root;
    }
    struct TreeNode *l = invertTree(root->right);
    struct TreeNode *r = invertTree(root->left);
    root->left = l;
    root->right = r;
    return root;
}