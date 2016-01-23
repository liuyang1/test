#ifndef LEET_H_
#define LEET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define TLOG(tag, fmt, ...) printf("%s:%d %s " fmt, \
                                   __FUNCTION__, __LINE__, tag, __VA_ARGS__);
// #define LOG(fmt, ...) printf("%s:%d "fmt, __FUNCTION__, __LINE__, __VA_ARGS__);
#ifndef LOG
#define LOG(...)
#endif
#define DIE(...) fprintf(stderr, ">>> " __VA_ARGS__);

#define SBOOL(x) ((x) ? "true" : "false")

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

struct ListNode {
    int val;
    struct ListNode *next;
};

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeLinkNode {
 int val;
 struct TreeLinkNode *left, *right, *next;
};

void showArr(int *nums, int numsSize);
void showLst(struct ListNode *head);
struct ListNode *buildLst(int *nums, int numsSize);
void reallocM(void *pp, int sz);

#define COLORFUL
#ifdef COLORFUL
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0;m"
#else
#define RED
#define GREEN
#define RESET
#endif
static inline const char *expect(bool v) {
    return v ? (GREEN "PASS"RESET) : (RED "FAIL"RESET);
}

#ifndef NO_IMPL
#include <stdio.h>
#include <stdlib.h>

struct ListNode *buildLst(int *nums, int numsSize) {
    struct ListNode *lst, *p;
    if (numsSize == 0) {
        return NULL;
    }
    int i;
    for (i = 0, p = malloc(sizeof(struct ListNode)), lst = p;
         i != numsSize - 1;
         i++, p->next = malloc(sizeof(struct ListNode)), p = p->next) {
        p->val = nums[i];
    }
    p->val = nums[numsSize - 1];
    p->next = NULL;
    return lst;
}

bool cmpLst(struct ListNode *a, struct ListNode *b) {
    for (; a && b; a = a->next, b = b->next) {
        if (a->val != b->val) {
            return false;
        }
    }
    return a == NULL && b == NULL;
}

// As C is static-typed, adding this magic number to represent as NIL
#define NIL         (0x0eadbeef)
typedef struct {
    void **a;
    int sz;
    int b, e;
} Q;

// basicly queue, but not full test, espcially when queue is full
static inline void q_init(Q *q, int sz) {
    q->a = malloc(sizeof(void *) * sz);
    q->sz = sz;
    q->b = q->e = 0;
}

static inline void q_deinit(Q *q) {
    if (q->a) {
        free(q->a);
    }
}

static inline int q_next(Q *q, int idx) {
    return (idx + 1) % (q->sz);
}

static inline bool q_empty(Q *q) {
    return q->e == q->b;
}

static inline bool q_full(Q *q) {
    return q_next(q, q->e) == q->b;
}

static inline bool q_put(Q *q, void *p) {
    if (q_full(q)) {
        return false;
    }
    q->a[q->e] = p;
    q->e = q_next(q, q->e);
    return true;
}

// check is q_get fail by return value, may get NULL pointer,
//      it may good if had push NULL to Q
static inline bool q_get(Q *q, void **pp) {
    if (q_empty(q)) {
        *pp = NULL;
        return false;
    }
    *pp = q->a[q->b];
    q->b = q_next(q, q->b);
    return true;
}

// load tree from array as level order, which NIL present to null
// NOTICE, this level order may omit redutant NIL
//          1
//    NIL        2
//             3  NIL
// load as [1, NIL, 2, 3]
// omit trail NIL and not placeholder NIL comparing to full tree
struct TreeNode *buildTree(int *nums, int numsSize) {
    if (nums == NULL || numsSize <= 0) {
        return NULL;
    }
    struct TreeNode **a = malloc(sizeof(struct TreeNode *) * numsSize);
    int i, j;
    for (i = 0; i != numsSize; i++) {
        if (nums[i] != NIL) {
            a[i] = malloc(sizeof(struct TreeNode));
            a[i]->val = nums[i];
            a[i]->left = NULL; // add default NULL first
            a[i]->right = NULL;
        } else {
            a[i] = NULL;
        }
    }
    // i, parent pointer move one step
    // j, child pointer move two steps
    // inspried by StefanPochmann's python code
    // ref: https://leetcode.com/discuss/41182/tree-deserializer-and-visualizer-for-python
    for (i = 0, j = 1; i != numsSize; i++) {
        if (a[i] == NULL) {
            continue;
        }
        if (j < numsSize) {
            a[i]->left = a[j++];
        }
        if (j < numsSize) {
            a[i]->right = a[j++];
        }
    }
    struct TreeNode *p = a[0];
    free(a);
    return p;
}

// showTree with level order, may omit NIL
// reverse function of buildTree
void showTree(struct TreeNode *t) {
    Q qq, *q = &qq;
    q_init(q, 100);
    q_put(q, t);
    struct TreeNode *p;
    int nilcnt = 0, i, start = 1;
    printf("[");
    while (1) {
        if (!q_get(q, (void **)&p)) {
            break;
        }
        // use nilcnt to lazy print NIL, this will help not show trail NIL
        if (p == NULL) {
            nilcnt++;
        } else {
            for (i = 0; i != nilcnt; i++) {
                printf(", NIL");
            }
            nilcnt = 0;
            // use start flag to only show comman interlaced
            if (start != 1) {
                printf(", ");
            }
            printf("%d", p->val);
            start = 0;
            q_put(q, p->left);
            q_put(q, p->right);
        }
    }
    q_deinit(q);
    printf("]\n");
}

void showArr(int *nums, int numsSize) {
    int i;
    printf("[");
    for (i = 0; i != numsSize; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", nums[i]);
    }
    printf("]\n");
}

bool cmpArr(int *a0, int n0, int *a1, int n1) {
    if (n0 != n1) {
        return false;
    }
    int i;
    for (i = 0; i != n0; i++) {
        if (a0[i] != a1[i]) {
            return false;
        }
    }
    return true;
}

void showArr2(int **mat, int m, int n) {
    int i;
    printf("[\n");
    for (i = 0; i != m; i++) {
        showArr(mat[i], n);
    }
    printf("]\n");
}

void showLst(struct ListNode *p) {
    printf("[");
    int start;
    for (start = 1; p != NULL; p = p->next) {
        if (start != 0) {
            printf("%d", p->val);
            start = 0;
        } else {
            printf(", %d", p->val);
        }
    }
    printf("]\n");
}

void reallocM(void *pp, int sz) {
    void *origin = *(void **)pp;
    void *p = origin;
    p = realloc(origin, sz);
    *(void **)pp = p;
    if (p == NULL) {
        // when realloc fail, return p == NULL
        // directly p = realloc(p, sz); will memory leak
        free(origin);
        abort();
    }
}

void showTreeLink(struct TreeLinkNode *n) {
    struct TreeLinkNode *bro, *sibling, *nbro;
    for (bro = n; bro != NULL; bro = nbro) {
        nbro = NULL;
        for (sibling = bro; sibling != NULL; sibling = sibling->next) {
            if (nbro == NULL) {
                if (sibling->left) {
                    nbro = sibling->left;
                } else if (sibling->right) {
                    nbro = sibling->right;
                }
            }

            printf("%d, ", sibling->val);
        }
        printf("\n");
    }
}

void initTreeNode(struct TreeNode *n, int val,
              struct TreeNode *left, struct TreeNode *right) {
    n->val = val;
    n->left = left;
    n->right = right;
}
void initTreeLink(struct TreeLinkNode *n, int val,
                  struct TreeLinkNode *left,
                  struct TreeLinkNode *right) {
    n->val = val;
    n->left = left;
    n->right = right;
    n->next = NULL;
}

#endif
#endif
