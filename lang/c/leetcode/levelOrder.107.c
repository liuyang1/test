#include "leet.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
int height(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int l = height(root->left);
    int r = height(root->right);
    return MAX(l, r) + 1;
}

int **levelOrder(struct TreeNode *root, int **columnSizes, int *returnSize) {
    if (root == NULL) {
        *returnSize = 0;
        return NULL;
    }
    int h = height(root);
    int **rt = malloc(sizeof(int *) * h); // output
    int *sizes = malloc(sizeof(int) * h); // output

    // handle first one case
    int sz = 1;
    struct TreeNode **ls = malloc(sizeof(struct TreeNode *) * sz); // internal
    ls[0] = root;
    rt[0] = malloc(sizeof(int) * sz);
    rt[0][0] = root->val;
    sizes[0] = 1;
    // interate to next level
    int lvl;
    for (lvl = 1; lvl != h; lvl++) {
        rt[lvl] = malloc(sizeof(struct TreeNode *) * sz * 2); // output, over-allocation
        struct TreeNode **newls = malloc(sizeof(struct TreeNode *) * sz * 2); // internal
        int newsz, i;
        for (i = newsz = 0; i != sz; i++) {
            struct TreeNode *n = ls[i];
            if (n->left) {
                rt[lvl][newsz] = n->left->val;
                newls[newsz++] = n->left;
            }
            if (n->right) {
                rt[lvl][newsz] = n->right->val;
                newls[newsz++] = n->right;
            }
        }
        free(ls);
        ls = newls;
        sz = newsz;
        sizes[lvl] = newsz;
    }
    free(ls);
    *columnSizes = sizes;
    *returnSize = h;

    // swap to from bottom-up level
    int b, e, *t, tsz;
    for (b = 0, e = h - 1; b < e; b++, e--) {
        t = rt[b];
        rt[b] = rt[e];
        rt[e] = t;

        tsz = sizes[b];
        sizes[b] = sizes[e];
        sizes[e] = tsz;
    }
    return rt;
}

int main() {
    int a[] = {3, 9, 20, NIL, NIL, 15, 7};
    struct TreeNode *root = buildTree(a, COUNT_OF(a));
    int *sizes, sz;
    int **r = levelOrder(root, &sizes, &sz);
    showArr2v(r, sz, sizes);
    freeArr2v(r, sz, sizes);
    return 0;
}
