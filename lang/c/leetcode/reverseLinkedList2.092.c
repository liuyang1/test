#include "leet.h"

struct ListNode *reverseBetween(struct ListNode *head, int m, int n) {
    struct ListNode *enter = NULL, *leave = NULL, *prev, *next;
    struct ListNode *origin = head;
    if (m == n) {
        return origin;
    }
    int i;
    // m, n index begin with 1
    for (i = 1, prev = NULL; i != m; i++) {
        prev = head;
        head = head->next;
    }
    enter = prev;
    leave = head;
    LOG("enter=%p %d leave=%p %d\n", enter, enter ? enter->val : 0, leave, leave ? leave->val : 0);

    for (i = 0, prev = NULL; i <= n - m; i++) {
        if (i == n - m) {
            LOG("final %p %d\n", head, head ? head->val : 0);
            if (enter != NULL) {
                enter->next = head;
            } else {
                origin = head;
            }
            leave->next = head->next;
            head->next = prev;
            break;
        }
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return origin;
}

#define CASE(a, m, n, b)      { \
        struct ListNode *la = buildLst(a, sizeof(a) / sizeof(int)); \
        struct ListNode *lb = buildLst(b, sizeof(b) / sizeof(int)); \
        showLst(la); \
        la = reverseBetween(la, m, n); \
        showLst(la); showLst(lb); \
        printf("%s\n", expect(cmpLst(la, lb))); }

int main() {
    CASE(((int[]) {1, 2, 3, 4, 5}), 2, 4, ((int[]) {1, 4, 3, 2, 5}));
    CASE(((int[]) {1, 2, 3, 4, 5}), 1, 4, ((int[]) {4, 3, 2, 1, 5}));
    CASE(((int[]) {1, 2, 3, 4, 5}), 2, 5, ((int[]) {1, 5, 4, 3, 2}));
    CASE(((int[]) {1, 2, 3, 4, 5}), 1, 5, ((int[]) {5, 4, 3, 2, 1}));
    CASE(((int[]) {1, 2, 3, 4, 5}), 1, 3, ((int[]) {3, 2, 1, 4, 5}));
    CASE(((int[]) {1}), 1, 1, ((int[]) {1}));
    CASE(((int[]) {1, 2}), 1, 1, ((int[]) {1, 2}));
    CASE(((int[]) {1, 2}), 2, 2, ((int[]) {1, 2}));
    CASE(((int[]) {1, 2}), 1, 2, ((int[]) {2, 1}));
    CASE(((int[]) {3, 5}), 1, 2, ((int[]) {5, 3}));
    return 0;
}
