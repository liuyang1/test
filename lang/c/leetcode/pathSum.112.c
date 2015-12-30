/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
bool hasPathSum(struct TreeNode* root, int sum) {
    if (root == NULL) {
        return false; // return sum == 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return sum == root->val;
    }
    bool r = false;
    if (root->left != NULL) {
        r = hasPathSum(root->left, sum - root->val);
    }
    if (r == true) {
        return r;
    }
    if (root->right != NULL) {
        r = hasPathSum(root->right, sum - root->val);
    }
    return r;
}
