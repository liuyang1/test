/**
 * find cycle begin node if exist cycle in linked list
 *
 * head                slow & fast meet here
 *                     V
 * o -> o -> o -> o -> o -> o -+
 *               ^             |
 *               +< o <- o <---+
 * assume length of tail is A, length of ring in linked list is B.
 *      2(A + x) = A + B + x
 *      x = B - A
 * so when moving A step forward, get cycle begin node
 */
struct ListNode *detectCycle(struct ListNode *head) {
    struct ListNode *fast, *slow;
    for (fast = head, slow = head; fast && fast->next; ) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == NULL) {
            return NULL;
        }
        // fast, slow pointer meet
        if (fast == slow) {
            slow = head;
            // moveing A step forwarder, 0 + A = A, (x + A) % B = B % B = 0
            // find cycle begin node
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }
    return NULL;
}
