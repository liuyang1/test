/**
 * find if exist cycle in linked list
 */
bool hasCycle(struct ListNode *head) {
    struct ListNode *fast, *slow;
    for (fast = head, slow = head; fast && fast->next;) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == NULL) {
            return false;
        }
        if (fast == slow) {
            return true;
        }
    }
    return false;
}
