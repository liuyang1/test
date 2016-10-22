#ifndef LEET_H_
#define LEET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#define TLOG(tag, fmt, ...) printf("%s:%d %s " fmt, \
                                   __FUNCTION__, __LINE__, tag, __VA_ARGS__);
#define LOG(fmt, ...) printf("%s:%d "fmt, __FUNCTION__, __LINE__, __VA_ARGS__);
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
void freeLst(struct ListNode* head) {
    if (head == NULL) {
        return;
    }
    struct ListNode *next;
    for (; head; head = next) {
        next = head->next;
        free(head);
    }
}

int isSortedLst(struct ListNode *ls) {
    if (ls == NULL) {
        return true;
    }
    int val = ls->val;
    for (ls = ls->next; ls; ls = ls->next) {
        if (val > ls->val) {
            return false;
        } else {
            val = ls->val;
        }
    }
    return true;
}

struct ListNode *buildRandLst(int size) {
    int i, *r = malloc(sizeof(int) * size);
    for (i = 0; i != size; i++) {
        r[i] = rand();
    }
    free(r);
    struct ListNode *ls = buildLst(r, size);
    return ls;
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

// function on Tree {{{
// - buildTree
// - freeTree
// - showTree
// - eqTree

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

void freeTree(struct TreeNode *root) {
    if (root == NULL) {
        return;
    }
    if (root->left) {
        free(root->left);
    }
    if (root->right) {
        free(root->right);
    }
    free(root);
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

bool eqTree(struct TreeNode *t0, struct TreeNode *t1) {
    if (t0 == NULL && t1 == NULL) {
        return true;
    }
    if (t0 != NULL && t1 != NULL) {
        if (t0->val != t1->val) {
            return false;
        }
        return eqTree(t0->left, t1->left) && eqTree(t0->right, t1->right);
    }
    return false;
}
// }}} function on Tree END

void showArr(int *nums, int numsSize) {
    int i;
    printf("[");
    if (nums == NULL) {
        printf("nil");
    } else {
        for (i = 0; i != numsSize; i++) {
            if (i != 0) {
                printf(", ");
            }
            printf("%d", nums[i]);
        }
    }
    printf("]\n");
}

void strnArr(char *s, int n, int *nums, int numsSize) {
    int j = 0;
    j += snprintf(s + j, n - j, "[");
    if (nums == NULL) {
        j += snprintf(s + j, n - j, "nil");
    } else {
        int i;
        for (i = 0; i != numsSize; i++) {
            if (i != 0) {
                j += snprintf(s + j, n - j, ", ");
            }
            j += snprintf(s + j, n - j, "%d", nums[i]);
        }
    }
    j += snprintf(s + j, n - j, "]");
}

bool cmpArr(int *a0, int n0, int *a1, int n1) {
    if (n0 == 0 && n1 == 0) {
        return true;
    }
    if (a0 == NULL && a1 == NULL) {
        return true;
    } else if (a0 == NULL || a1 == NULL) {
        return false;
    }
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

// show arr which representation 2 dim by user
void showArr2c(int *mat, int m, int n) {
    int i;
    printf("[\n");
    for (i = 0; i != m; i++) {
        showArr(mat + n * i, n);
    }
    printf("]\n");
}

void showArr2v(int **mat, int m, int *n) {
    int i;
    printf("[\n");
    for (i = 0; i != m; i++) {
        printf("  ");
        showArr(mat[i], n[i]);
    }
    printf("]\n");
}

void freeArr2v(int **mat, int m, int *n) {
    int i;
    for (i = 0; i != m; i++) {
        free(mat[i]);
    }
    free(n);
    free(mat);
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

// math part
unsigned long long comb(unsigned long long n, unsigned long long a) {
    if (a > n - a) {
        a = n - a;
    }
    unsigned long long i, j, r = 1;
    for (i = n, j = 1; j != a + 1; i--, j++) {
        r = (r * i) / j;
    }
    return r;
}

// use INT type, when catalan(19), it will overflow
unsigned long long catalan(unsigned long long n) {
    return comb(2 * n, n) / (n + 1);
}

// math part END

void showStr(char *str) {
    printf("\'%s\'", str);
}
void showStr2(char **pstr, int n) {
    int i;
    bool start = true;
    printf("[");
    for (i = 0; i != n; i++) {
        if (!start) {
            printf(", ");
        }
        showStr(pstr[i]);
        start = false;
    }
    printf("]\n");
}

/**
 * REMIND: simply decrease it's wrong.
 * it may overflow.
 */
int cmpInt(const void *n0, const void *n1) {
    int a = *(int *)n0;
    int b = *(int *)n1;
    if (a > b) {
        return 1;
    } else if (a == b) {
        return 0;
    } else {
        return -1;
    }
}

#endif
#endif
