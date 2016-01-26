#include "leet.h"
int length(struct ListNode *head) {
    int cnt;
    for (cnt = 0; head; head = head->next) {
        cnt++;
    }
    return cnt;
}

struct ListNode *rotateRight(struct ListNode *head, int k) {
    int len = length(head);
    if (len == 0 || (k = k % len) == 0) {
        return head;
    }
    int i;
    struct ListNode *p, *newhead, *newtail;
    for (i = 0, p = head; i != len - k - 1; i++) {
        p = p->next;
    }
    newtail = p;
    newhead = p->next;
    newtail->next = NULL;
    for (p = newhead; p->next; p = p->next) {
    }
    p->next = head;
    return newhead;
}

void case0() {
    int nums[] = {1, 2, 3, 4, 5};
    struct ListNode *h = buildLst(nums, COUNT_OF(nums));
    showLst(h);
    int i;
    for (i = 0; i != 7; i++) {
        h = rotateRight(h, 2);
        printf("rorate %2d: ", i); showLst(h);
    }
}

void case1() {
    int nums[] = {};
    struct ListNode *h = buildLst(nums, COUNT_OF(nums));
    showLst(h);
    h = rotateRight(h, 0); showLst(h);
}

int main() {
    case0();
    case1();
    return 0;
}
