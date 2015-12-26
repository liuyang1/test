#include <stdlib.h>
#include <stdio.h>
// #define LOG(...) printf(__VA_ARGS__);
#define LOG(...)
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;
/**
 * Definition for singly-linked list.
 */
int recDepth(struct ListNode *head, int n) {
    if (head == NULL) {
        return 0;
    }
    int dep = recDepth(head->next, n);
    if (dep == n) {
        LOG("del n=%d val=%d\n", n, head->next->val);
        free(head->next);
        head->next = head->next->next;
    }
    return dep + 1;
}

struct ListNode *removeNthFromEnd(struct ListNode *head, int nth) {
    if (head == NULL || nth <= 0) {
        return head;
    }
    int depth = recDepth(head, nth);
    if (depth == nth) {
        LOG("del n=%d val=%d head\n", nth, head->val);
        free(head);
        return head->next;
    }
    return head;
}

void showLst(ListNode *head)
{
    printf("[");
    int start;
    for (start = 1; head != NULL; head = head->next) {
        if (start != 1) {
            printf(", ");
        } else {
            start = 0;
        }
        printf("%d", head->val);
    }
    printf("]");
}

int case0(int *a, int n, int nth)
{
    ListNode *lst;
    if (n == 0) {
        lst = NULL;
        goto test;
    }
    ListNode *head;
    int i;
    for (i = 0, head = malloc(sizeof(ListNode)), lst = head;
         i != n - 1;
         i++, head->next = malloc(sizeof(ListNode)), head = head->next) {
        head->val = a[i];
    }
    head->val = a[n - 1];
    head->next = NULL;

test:
    printf("case:\t"); showLst(lst); printf(" remove nth=%d\n", nth);
    lst = removeNthFromEnd(lst, nth);
    showLst(lst); printf("\n");
    return 0;
}

#define CASE(a, nth) case0(a, sizeof(a) / sizeof(int), nth);
int main()
{
    CASE(((int[]) {2, 3, 4}), 0);
    CASE(((int[]) {2, 3, 4}), 1);
    CASE(((int[]) {2, 3, 4}), 2);
    CASE(((int[]) {2, 3, 4}), 3);
    CASE(((int[]) {2, 3, 4}), 4);
    CASE(((int[]) {2}), 1);
    CASE(((int[]) {2}), 2);
    CASE(((int[]) {}), 1);
    return 0;
}
