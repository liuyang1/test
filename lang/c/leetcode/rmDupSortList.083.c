/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* deleteDuplicates(struct ListNode* head) {
    struct ListNode *p0 = NULL, *h0 = head;
    for (; head != NULL; head = head->next) {
        if (p0 == NULL) {
            p0 = head;
        } else {
            if (p0->val == head->val) {
                // skip this `head`
            } else {
                p0->next = head;
                p0 = head;
            }
        }
    }
    if (p0 != NULL) {
        p0->next = NULL;
    }
    return h0;
}
