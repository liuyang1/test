#ifndef LEET_H_
#define LEET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef __cplusplus // to pass compile under pure C
typedef int bool;
#define true 1
#define false 0
#endif

struct ListNode {
    int val;
    struct ListNode *next;
};

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void showArr(int *nums, int numsSize);
void showLst(struct ListNode *head);
struct ListNode *buildLst(int *nums, int numsSize);
void reallocM(void *pp, int sz);

#ifndef NO_IMPL
#include <stdio.h>
#include <stdlib.h>

struct ListNode *buildLst(int *nums, int numsSize)
{
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

// As C is static-typed, adding this magic number to represent as NIL
#define NIL         (0x0eadbeef)
typedef struct {
    void **a;
    int sz;
    int b, e;
} Q;

// basicly queue, but not full test, espcially when queue is full
inline void q_init(Q *q, int sz) {
    q->a = malloc(sizeof(void *) * sz);
    q->sz = sz;
    q->b = q->e = 0;
}
inline void q_deinit(Q *q) {
    if (q->a) {
        free(q->a);
    }
}
inline int q_next(Q *q, int idx) {
    return (idx + 1) % (q->sz);
}
inline bool q_empty(Q *q) {
    return q->e == q->b;
}
inline bool q_full(Q *q) {
    return q_next(q, q->e) == q->b;
}
inline bool q_put(Q *q, void *p) {
    if (q_full(q)) {
        return false;
    }
    q->a[q->e] = p;
    q->e = q_next(q, q->e);
    return true;
}
// check is q_get fail by return value, may get NULL pointer,
//      it may good if had push NULL to Q
inline bool q_get(Q *q, void **pp) {
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
    int cnt, i;
    for (i = 0; i != numsSize; i++) {
        if (nums[i] != NIL) {
            a[i] = malloc(sizeof(struct TreeNode));
            a[i]->val = nums[i];
        } else {
            a[i] = NULL;
        }
    }
    Q qq, *q = &qq;
    q_init(q, numsSize);
    q_put(q, a[0]);
    i = 1;
    struct TreeNode *p;
    while (1) {
        q_get(q, &p);
        if (p == NULL) {
            break;
        }
        if (i >= numsSize) {
            p->left = NULL;
            p->right = NULL;
        } else {
            p->left = a[i];
            p->right = a[i + 1];
            i += 2;
        }
        if (p->left) {
            q_put(q, p->left);
        }
        if (p->right) {
            q_put(q, p->right);
        }
    }

    q_deinit(q);
    p = a[0];
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
        if (!q_get(q, &p)) {
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

#endif
#endif
