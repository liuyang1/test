#include "leet.h"
struct ListNode* mergeList(struct ListNode *l0, struct ListNode *l1) {
    struct ListNode *h = NULL, *p = NULL, *min;
    if (l0 == NULL) {
        return l1;
    } else if (l1 == NULL) {
        return l0;
    }
    while (l0 && l1) {
        if (l0->val <= l1->val) {
            min = l0;
            l0 = l0->next;
        } else {
            min = l1;
            l1 = l1->next;
        }
        if (p == NULL) {
            h = p = min;
        } else {
            p->next = min;
            p = min;
        }
    }
    if (l0) {
        p->next = l0;
    }
    if (l1) {
        p->next = l1;
    }
    return h;
}
struct ListNode* sortList(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    struct ListNode *slow, *fast, *slow1;
    for (slow = head, fast = head;
         fast && fast->next;
         slow1 = slow,
         slow = slow->next,
         fast = fast->next->next) {
    }
    struct ListNode *half = slow1->next;
    slow1->next = NULL;
    head = sortList(head);
    half = sortList(half);
    head = mergeList(head, half);
    return head;
}

#define CASE(a) {struct ListNode *ls = buildLst(a, COUNT_OF(a)); \
    ls = sortList(ls); showLst(ls); printf("%s\n", expect(isSortedLst(ls))); \
    freeLst(ls); }

#define REGION 1024
bool randTest() {
    int sz = rand() % REGION;
    struct ListNode *ls = buildRandLst(sz);
    ls = sortList(ls);
    bool e = isSortedLst(ls);
    freeLst(ls);
    return e;
}
int main() {
    CASE(((int []) {0, 1, 9, 3, 7, 5, 8, 4}));
    CASE(((int []) {1, 0}));
    CASE(((int []) {1, 0, 3, 2}));
    CASE(((int []) {1, 0, 3, 2}));
    int i;
    for (i = 0; i != 10; i++) {
        bool r = randTest();
        if (r) {
            printf("randTest #%d: %s\n", i, expect(r));
        } else {
            break;
        }
    }
    return 0;
}
