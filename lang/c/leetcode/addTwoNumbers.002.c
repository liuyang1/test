/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    int carry = 0, sum;
    struct ListNode *head = (struct ListNode *)malloc(sizeof(struct ListNode));
    struct ListNode *tail = head;
    while (1) {
        sum = carry;
        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }
        tail->val = sum % 10;
        carry = sum / 10;
        if (carry == 0 && l1 == NULL && l2 == NULL) {
            tail->next = NULL;
            break;
        }
        tail->next = (struct ListNode *)malloc(sizeof(struct ListNode));
        tail = tail->next;
    }
    return head;
}
