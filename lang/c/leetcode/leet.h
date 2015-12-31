#ifndef LEET_H_
#define LEET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
