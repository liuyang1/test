/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* swapPairs(struct ListNode* head) {
    struct ListNode *p0, *p1, *pp = NULL, *ret = head, *ori = head;
    int i;
    if (head == NULL) {
        return NULL;
    }
    // PrevNode EvenNode OddNode NextNode
    // pp       p0       p1/head
    //          p1       p0/head/pp
    for (i = 0; head != NULL; i++, head = head->next) {
        if (i % 2 == 0) {
            p0 = head;
        } else {
            p1 = head;
            if (pp != NULL) {
                pp->next = p1;
            } else if (ret == ori) {
                ret = p1;
            }
            p0->next = p1->next;
            p1->next = p0;
            head = p0;
            pp = p0;
        }
    }
    return ret;
}
