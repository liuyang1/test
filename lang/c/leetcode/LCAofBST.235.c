#include "leet.h"
bool isChild(struct TreeNode *root, struct TreeNode *n) {
    if (root == NULL) {
        return false;
    }
    return root == n || isChild(root->left, n) || isChild(root->right, n);
}

struct TreeNode *lowestCommonAncestor(struct TreeNode *root,
                                      struct TreeNode *p, struct TreeNode *q) {
    if (root == NULL) {
        return NULL;
    }
    struct TreeNode *t = lowestCommonAncestor(root->left, p, q);
    if (t) {
        return t;
    }
    t = lowestCommonAncestor(root->right, p, q);
    if (t) {
        return t;
    }
    bool lp, lq, rp, rq;
    lp = isChild(root->left, p);
    lq = isChild(root->left, q);
    rp = isChild(root->right, p);
    rq = isChild(root->right, q);
    if ((lp && rq) ||
        (rp && lq) ||
        ((lp || rp) && root == q) ||
        (((lq || rq) && root == p))) {
        return root;
    }
    return NULL;
}

int test0() {
/*
        _____0(6)_____
       /              \
    _1(2)_          _2(8)_
   /      \        /      \
   3(0)   _4(4)  5(7)     6(9)
         /  \
       7(3) 8(5)
 */
    struct TreeNode *at = malloc(sizeof(struct TreeNode) * 9);
    initTreeNode(at + 0, 6, at + 1, at + 2);
    initTreeNode(at + 1, 2, at + 3, at + 4);
    initTreeNode(at + 2, 8, at + 5, at + 6);
    initTreeNode(at + 3, 0, NULL, NULL);
    initTreeNode(at + 4, 4, at + 7, at + 8);
    initTreeNode(at + 5, 7, NULL, NULL);
    initTreeNode(at + 6, 9, NULL, NULL);
    initTreeNode(at + 7, 3, NULL, NULL);
    initTreeNode(at + 8, 5, NULL, NULL);

#define EXPECT(r, p, q, e)    { \
        struct TreeNode *t = lowestCommonAncestor(at + r, at + p, at + q); \
        printf("%p ?= %p %s\n", \
               t, at + e, expect(t == ((e < 0) ? NULL : at + e))); \
}

    EXPECT(0, 1, 8, 1);
    EXPECT(1, 1, 8, 1);
    EXPECT(0, 3, 5, 0);
    EXPECT(0, 3, 4, 1);
    EXPECT(0, 7, 8, 4);
    EXPECT(1, 7, 8, 4);
    EXPECT(1, 7, 5, -1);
    EXPECT(0, 0, 1, 0);
    EXPECT(0, 1, 2, 0);
    EXPECT(0, 2, 1, 0);

    return 0;
}

int main() {
    test0();
    return 0;
}
