#ifndef LEET_H_
#define LEET_H_

struct ListNode {
    int val;
    struct ListNode *next;
};

void showArr(int *nums, int numsSize);
void showLst(struct ListNode *head);
struct ListNode *buildLst(int *nums, int numsSize);
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
#endif
#endif
