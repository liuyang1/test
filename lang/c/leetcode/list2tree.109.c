#include "leet.h"

int list_length(struct ListNode *head) {
    int len;
    for (len = 0; head != NULL; len++, head = head->next) {
    }
    return len;
}

struct ListNode *list_index(struct ListNode *head, int idx) {
    int i;
    for (i = 0; head != NULL; i++, head = head->next) {
        if (i == idx) {
            return head;
        }
    }
    return NULL;
}

// This is bad performance, keep same formal as ar2tree.108.c
struct TreeNode *buildBSTree(struct ListNode *head, int len) {
    if (head == NULL || len == 0) {
        return NULL;
    }
    int idx = len / 2; // prefer this one, left compact
    // int idx = (numsSize - 1) / 2; // to match leetcode's result
    struct TreeNode *n = malloc(sizeof(struct TreeNode));
    struct ListNode *ln = list_index(head, idx);
    n->val = ln->val;
    n->left = buildBSTree(head, idx);
    n->right = buildBSTree(ln->next, len - idx - 1);
    return n;
}

struct TreeNode *sortedListToBST(struct ListNode *head) {
    int len = list_length(head);
    return buildBSTree(head, len);
}

#define CASE(a) {struct ListNode *ls = buildLst(a, COUNT_OF(a)); \
                 struct TreeNode *r = sortedListToBST(ls);       \
                 showTree(r); }
int main() {
    CASE(((int[]) {1, 2, 3, 4, 5}));
    CASE(((int[]) {1, 2, 3, 4, 5, 6, 7, 8}));
    return 0;
}
