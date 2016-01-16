#include "leet.h"
// use O(h) memory, so use DFS
typedef struct TreeNode *T;
const int stack_step = 1;
typedef struct {
    T *stk;
    int dep;
    int len;
} Stack;

Stack *stack_create() {
    Stack *p = malloc(sizeof(Stack));
    p->stk = malloc(sizeof(T *) * stack_step);
    p->len = stack_step;
    p->dep = 0;
    return p;
}

void stack_destroy(Stack *p) {
    if(p->stk) {
        free(p->stk);
    }
    free(p);
}

bool stack_empty(Stack *p) {
    return p->dep == 0;
}

void stack_push(Stack *p, T t) {
    p->dep++;
    if (p->dep == p->len) {
        p->len += stack_step;
        T *rp = realloc(p->stk, sizeof(T *) * p->len);
        if (rp == NULL) {
            free(p->stk);
            // should not here
        }
        p->stk = rp;
    }
    LOG("push %d at %d\n", t->val, p->dep);
    p->stk[p->dep] = t;
}

bool stack_pop(Stack *p, T *t) {
    if (stack_empty(p)) {
        if (t != NULL) {
            *t = NULL;
        }
        return false;
    }
    if (t != NULL) {
        *t = p->stk[p->dep];
    }
    LOG("pop  %d at %d\n", p->stk[p->dep]->val, p->dep);
    p->dep--;
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
    CASE(((int []){}));
    CASE(((int []){1, 2}));
    CASE(((int []){1, 2, 3}));
    CASE(((int []){1, 2, NIL, 3}));
    CASE(((int []){1, 2, 3, NIL, 4}));
    CASE(((int []){4, 2, 5, 1, 3}));
    return 0;
}
