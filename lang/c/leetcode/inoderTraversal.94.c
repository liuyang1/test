#include "leet.h"

typedef struct {
    int *a;
    int sz;
} Ctx;

void initCtx(Ctx *ctx) {
    ctx->a = NULL;
    ctx->sz = 0;
}
void deinitCtx(Ctx *ctx) {
    if (ctx->a) {
        free(ctx->a);
    }
}
void output(Ctx *ctx, int val) {
    ctx->sz++;
    reallocM(&(ctx->a), sizeof(int) * ctx->sz);
    ctx->a[ctx->sz - 1] = val;
}

void inorder(Ctx *ctx, struct TreeNode *root) {
    if (root == NULL) {
        return;
    }
    inorder(ctx, root->left);
    output(ctx, root->val);
    inorder(ctx, root->right);
}
/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *inorderTraversal(struct TreeNode *root, int *returnSize) {
    Ctx ctx, *pctx = &ctx;
    initCtx(pctx);
    inorder(pctx, root);
    *returnSize = pctx->sz;
    return pctx->a;
}

#define CASE(a) { \
    struct TreeNode *t = buildTree(a, sizeof(a) / sizeof(int)); \
    showTree(t); \
    int sz, *r = inorderTraversal(t, &sz); \
    showArr(r, sz); free(r); freeTree(t); \
}
int main() {
    CASE(((int []){1, 2, NIL, 3}));
    CASE(((int []){1, NIL, 2, 3}));
    CASE(((int []){1, 2, 3}));
    CASE(((int []){1, 2, NIL, NIL, 3}));
    CASE(((int []){1, NIL, 2, NIL, 3}));
    CASE(((int []){37,-34,-48,NIL,-100,-100,48,NIL,NIL,NIL,NIL,-54,NIL,-71,-22,NIL,NIL,NIL,8}));

    return 0;
}
