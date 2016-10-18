/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode *l0 = NULL, *p0 = NULL, *p = NULL;
    while (l1 != NULL && l2 != NULL) {
        if (l1->val < l2->val) {
            p = l1;
            l1 = l1->next;
        } else {
            p = l2;
            l2 = l2->next;
        }
        if (p0 != NULL) {
            p0->next = p;
        } else {
            l0 = p;
        }
        p0 = p;
    }
    if (l1 != NULL) {
        p = l1;
    }
    if (l2 != NULL) {
        p = l2;
    }
    if (p0 != NULL) {
        p0->next = p;
    } else {
        l0 = p;
    }
    p0 = p;
    return l0;
}
