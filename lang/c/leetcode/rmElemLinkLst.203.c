#include <stdio.h>
#include "leet.h"

struct ListNode* removeElements(struct ListNode* head, int val) {
    struct ListNode *p, *p1, *p0;
    for (p = head; p != NULL; p = p1) {
        p1 = p->next;
        if (p->val != val) { 
            p0 = p;
            continue;
        }
        if (p == head) {
            head = p1;
        } else {
            p0->next = p1;
        }
        free(p);
    }
    return head;
}


#define CASE(a, v) {struct ListNode *l = buildLst(a, sizeof(a) / sizeof(int)); \
    showLst(l); \
    l = removeElements(l, v); showLst(l); freeLst(l); }
int main()
{
    CASE(((int []){0, 1, 2}), 0);
    CASE(((int []){0, 1, 2}), 1);
    CASE(((int []){0, 1, 2}), 2);
    CASE(((int []){0, 1, 2}), 4);
    CASE(((int []){0, 1, 3, 0, 12}), 0);
    CASE(((int []){}), 1);
    CASE(((int []){1, 2, 2, 1}), 2);
    CASE(((int []){1, 2, 2, 1}), 1);
    return 0;
}
