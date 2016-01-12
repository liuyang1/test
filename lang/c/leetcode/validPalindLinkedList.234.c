#include "leet.h"
struct ListNode *reverseList(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    struct ListNode *prev = NULL, *next;
    while (head != NULL) {
        next = head->next; head->next = prev;
        prev = head; head = next;
    }
    return prev;
}

// NOTICE, this half will around to tail
// A -> B -> C -> D -> nil, it will return pointer to C
// A -> B -> C -> nil, it will return pointer to C
struct ListNode *halfList(struct ListNode * head) {
    struct ListNode *fast, *slow;
    for (fast = head, slow = head;
         fast && fast->next;
         fast = fast->next->next, slow = slow->next) {
    }
    if (fast && fast->next == NULL) {
        return slow->next; // move one more step to around to tail
    }
    return slow;
}

bool isPalindrome(struct ListNode *head) {
    struct ListNode *p0, *p1;
    struct ListNode *half = halfList(head);
    half = reverseList(half);
    bool r = true;
    for (p0 = head, p1 = half; p0 && p1; p0 = p0->next, p1 = p1->next) {
        if (p0->val != p1->val) {
            r = false;
            break;
        }
    }
    // reverse back to keep list not modify
    half = reverseList(half);
    return r;
}

#define CASE(a, e) {struct ListNode *lst = buildLst(a, sizeof(a) / sizeof(int)); \
    showLst(lst); \
    bool r = isPalindrome(lst); \
    printf("%s ?= %s %s\n", SBOOL(r), SBOOL(e), expect(r == e)); }

int main() {
    CASE(((int []){}), true);
    CASE(((int []){1}), true);
    CASE(((int []){1, 1}), true);
    CASE(((int []){1, 2, 1}), true);
    CASE(((int []){1, 2, 3}), false);
    CASE(((int []){1, 2, 3, 2, 1}), true);
    CASE(((int []){1, 2, 3, 3, 2, 1}), true);
    return 0;
}
