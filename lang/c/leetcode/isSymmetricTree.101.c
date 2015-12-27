/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
bool isSymmetricTwo(struct TreeNode *left, struct TreeNode *right) {
    if (left == NULL && right == NULL) {
        return true;
    }
    if (left == NULL || right == NULL) {
        return false;
    }
    if (left->val != right->val) {
        return false;
    }
    return isSymmetricTwo(left->left, right->right) && isSymmetricTwo(left->right, right->left);
}
bool isSymmetric(struct TreeNode* root) {
    if (root == NULL) {
        return true;
    }
    return isSymmetricTwo(root->left, root->right);
}
