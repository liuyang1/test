#include "leet.h"
void deleteNode(struct ListNode *node) {
    node->val = node->next->val;
    struct ListNode *n = node->next;
    node->next = node->next->next;
    free(n);
}
