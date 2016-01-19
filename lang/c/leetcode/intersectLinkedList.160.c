#include "leet.h"

int countNode(struct ListNode *p) {
    int c = 0;
    for (; p != NULL; p = p->next, c++) {
    }
    return c;
}

struct ListNode *getIntersectionNode(struct ListNode *headA,
                                     struct ListNode *headB) {
    int ca = countNode(headA);
    int cb = countNode(headB);
    int ct, i;
    struct ListNode *t;
    if (ca < cb) {
        t = headA; headA = headB; headB = t;
        ct = ca; ca = cb; cb = ct;
    }
    for (i = ca - cb; i != 0; i--, headA = headA->next) {
    }
    for (i = cb; i != 0; i--, headA = headA->next, headB = headB->next) {
        if (headA == headB || headA->val == headB->val) {
            return headA;
        }
    }
    return NULL;
}

int test0() {
    int a0[] = {3};
    int a1[] = {2, 3};
    struct ListNode *h0 = buildLst(a0, sizeof(a0) / sizeof(int));
    struct ListNode *h1 = buildLst(a1, sizeof(a1) / sizeof(int));
    struct ListNode *p = getIntersectionNode(h0, h1);
    if (p) {
        printf("%p %d\n", p, p->val);
    } else {
        printf("NULL\n");
    }
    return 0;
}

int main() {
    test0();
    return 0;
}
