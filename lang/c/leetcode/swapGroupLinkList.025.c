/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    if (head == NULL || k <= 1) {
        free(p);
        return head;
    }
    struct ListNode *pp = NULL, *ret = head, *ori = head;
    struct ListNode **p = malloc(sizeof(struct ListNode *) * k);
    int i, j;
    // PrevGroup Node0 Node1 Node2 NextGroup
    // pp       p0     p1    p2/head
    //          p2     p1    p0/head/pp
    for (i = 0; head != NULL; i++, i %= k, head = head->next) {
        p[i] = head;
        if ((i + 1) % k == 0) {
            if (pp != NULL) {
                pp->next = p[k - 1];
            } else if (ret == ori) {
                ret = p[k - 1];
            }
            p[0]->next = p[k - 1]->next;
            for (j = k - 1; j > 0; j--) {
                p[j]->next = p[j - 1];
            }
            head = p[0];
            pp = p[0];
        }
    }
    free(p);
    return ret;
}
