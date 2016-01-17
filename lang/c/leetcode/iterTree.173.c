#include "leet.h"
// use O(h) memory, so use DFS
/* define STACK with template type T
 * This version for complex type T, it will save T pointer at stack, not value.
 * so NOT save simple type T, such as INT to stack.
 */
typedef struct TreeNode *T;
const int stack_step = 1;
typedef struct {
    T *stk;
    int dep;
    int len;
} Stack;

// #define MEMLOG(...)     TLOG("MEM", __VA_ARGS__);
#ifndef MEMLOG
#define MEMLOG
#endif
// simple STACK, only increase stack size, never decrease.
void stack_in_extend(Stack *p) {
    p->len += stack_step;
    T *op = p->stk;
    p->stk = realloc(p->stk, sizeof(T *) * p->len);
    if (p->stk == NULL) {
        free(op);
        DIE("fail realloc, free old p->stk=%p\n", op);
        // memory not enough, should not here
    }
    MEMLOG("realloc p->stk=%p size=%lu\n", p->stk, sizeof(T *) * p->len);
}

Stack *stack_create() {
    Stack *p = malloc(sizeof(Stack));
    p->stk = NULL;
    p->len = 0;
    p->dep = 0;
    return p;
}

void stack_destroy(Stack *p) {
    if (p->stk) {
        MEMLOG("free p->stk=%p\n", p->stk);
        free(p->stk);
    }
    free(p);
}

bool stack_empty(Stack *p) {
    return p->dep == 0;
}

void stack_push(Stack *p, T t) {
    if (p->dep == p->len) {
        stack_in_extend(p);
    }
    p->stk[p->dep] = t;
    LOG("push %d at %d %p\n", t->val, p->dep, p->stk + p->dep);
    p->dep++;
}

bool stack_pop(Stack *p, T *t) {
    if (stack_empty(p)) {
        if (t != NULL) {
            *t = NULL;
        }
        return false;
    }
    p->dep--;
    if (t != NULL) {
        *t = p->stk[p->dep];
    }
    LOG("pop  %d at %d %p\n", p->stk[p->dep]->val, p->dep, p->stk + p->dep);
    return true;
}

struct BSTIterator {
    Stack *p;
};

struct BSTIterator *bstIteratorCreate(struct TreeNode *root) {
    struct BSTIterator *iter = malloc(sizeof(struct BSTIterator));
    iter->p = stack_create();
    for (; root; root = root->left) {
        stack_push(iter->p, root);
    }
    return iter;
}

/** @return whether we have a next smallest number */
bool bstIteratorHasNext(struct BSTIterator *iter) {
    return !stack_empty(iter->p);
}

/** @return the next smallest number */
int bstIteratorNext(struct BSTIterator *iter) {
    struct TreeNode *t, *root;
    if (stack_pop(iter->p, &t)) {
        if (t->right) {
            for (root = t->right; root; root = root->left) {
                stack_push(iter->p, root);
            }
        }
        return t->val;
    }
    return NIL;
}

/** Deallocates memory previously allocated for the iterator */
void bstIteratorFree(struct BSTIterator *iter) {
    if (iter->p) {
        stack_destroy(iter->p);
    }
    free(iter);
}

int iterTree(struct TreeNode *t) {
    struct BSTIterator *iter = bstIteratorCreate(t);
    bool start = true;
    printf("[");
    while (bstIteratorHasNext(iter)) {
        if (start != true) {
            printf(", ");
        } else {
            start = false;
        }
        printf("%d", bstIteratorNext(iter));
    }
    printf("]\n");
    bstIteratorFree(iter);
    return 0;
}

#define CASE(a) {struct TreeNode *t = buildTree(a, sizeof(a) / sizeof(int)); \
                 printf("show: "); showTree(t); \
                 printf("iter: "); iterTree(t); }
int main() {
    CASE(((int[]) {}));
    CASE(((int[]) {1, 2}));
    CASE(((int[]) {1, 2, 3}));
    CASE(((int[]) {1, 2, NIL, 3}));
    CASE(((int[]) {1, 2, 3, NIL, 4}));
    CASE(((int[]) {4, 2, 5, 1, 3}));
    return 0;
}
