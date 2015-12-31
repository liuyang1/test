#include "leet.h"

typedef struct {
    struct TreeNode **stack;
    int dep;
    int maxdep;
} Stack;

void initStack(Stack *stk) {
    stk->stack = NULL;
    stk->dep = 0;
    stk->maxdep = 0;
}

void deinitStack(Stack *stk) {
    if (stk->stack != NULL) {
        free(stk->stack);
    }
}

void push(Stack *stk, struct TreeNode *p) {
    if (p != NULL) {
        stk->dep++;
        if (stk->dep > stk->maxdep) {
            stk->maxdep = stk->dep;
            reallocM(&(stk->stack), sizeof(struct TreeNode *) * stk->maxdep);
        }
        stk->stack[stk->dep - 1] = p;
    }
}

void pop(Stack *stk, struct TreeNode **p) {
    if (p == NULL) {
        // calling's bug, should never happen
        assert(0);
        return;
    }
    if (stk->dep > 0) {
        *p = stk->stack[stk->dep - 1];
        stk->dep--;
    } else {
        *p = NULL;
    }
}

/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *preorderTraversal(struct TreeNode *root, int *returnSize) {
    int *r = NULL, sz = 0;
    Stack stack, *pstk = &stack;
    initStack(pstk);

    push(pstk, root);
    struct TreeNode *p;
    while (1) {
        pop(pstk, &p);
        if (p == NULL) {
            break;
        }
        push(pstk, p->right);
        push(pstk, p->left); // left first, when pop
        sz++;
        reallocM(&r, sizeof(int) * sz);
        r[sz - 1] = p->val;
    }

    deinitStack(pstk);
    *returnSize = sz;
    return r;
}

void initNode(struct TreeNode *n, int val,
              struct TreeNode *left, struct TreeNode *right) {
    n->val = val;
    n->left = left;
    n->right = right;
}

int main() {
    struct TreeNode n3, n2, n1;
    initNode(&n3, 3, NULL, NULL);
    initNode(&n2, 2, &n3, NULL);
    initNode(&n1, 1, NULL, &n2);

    int sz;
    int *r = preorderTraversal(&n1, &sz);
    showArr(r, sz);
    free(r);

    return 0;
}
