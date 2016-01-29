#include "leet.h"
typedef enum {
    even = 0,
    odd = 1,
} Parity;
struct ListNode* oddEvenList(struct ListNode* head) {
    Parity parity;
    struct ListNode *p0, *p1, *h0, *h1, *p;
    for (p = head, p0 = p1 = h0 = h1 = NULL, parity = even;
         p;
         p = p->next, parity = (parity == even ? odd : even)) {
        if (parity == even) {
            if (!h0) {
                h0 = p;
            }
            if (p0) {
                p0->next = p;
            }
            p0 = p;
        } else {
            if (!h1) {
                h1 = p;
            }
            if (p1) {
                p1->next = p;
            }
            p1 = p;
        }
    }
    if (p1) {
        p1->next = NULL;
    }
    if (p0) {
        p0->next = h1;
    }
    return h0;
}

#define CASE(a) {struct ListNode *lst = buildLst(a, sizeof(a) / sizeof(int)); \
    lst = oddEvenList(lst); showLst(lst); freeLst(lst); }
int main() {
    CASE(((int []) {1, 2, 3, 4}));
    CASE(((int []) {1, 2, 3}));
    CASE(((int []) {1, 2}));
    CASE(((int []) {1}));
    CASE(((int []) {}));
    return 0;
}
