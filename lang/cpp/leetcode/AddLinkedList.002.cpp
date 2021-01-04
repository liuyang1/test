#include <assert.h>
#include <iostream>

static const int base = 10;
/**
 * Definition for singly-linked list.
 * Least-significant digit comes first
 */
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {
        if (x < 0) {
            assert(0);
        }
        if (x < base) {
            val = x, next = nullptr;
        } else {
            val = x % base, next = new ListNode(x / base);
        }
    }
    ListNode(int x, ListNode *next) : val(x), next(next) {}
    ~ListNode() {
        if (next) {
            delete next;
        }
    }
    void show() {
        printf("%d", val);
        if (next != NULL) {
            next->show();
        }
    }
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *r = new ListNode(), *p = r;
        int carry;
        for (carry = 0; l1 != NULL && l2 != NULL; l1 = l1->next, l2 = l2->next) {
            carry += l1->val + l2->val;
            p->val = carry % base, carry /= base;
            p->next = new ListNode();
            p = p->next;
        }
        for (; l1 != NULL; l1 = l1->next) {
            carry += l1->val;
            p->val = carry % base, carry /= base;
            p->next = new ListNode();
            p = p->next;
        }
        for (; l2 != NULL; l2 = l2->next) {
            carry += l2->val;
            p->val = carry % base, carry /= base;
            p->next = new ListNode();
            p = p->next;
        }
        if (carry != 0) {
            p->val = carry;
        } else {
            ListNode *h, *l;
            for (h = r, l = NULL; h->next != NULL; h = h->next) {
                l = h;
            }
            if (l != NULL) {
                delete l->next;
                l->next = NULL;
            }
        }
        return r;
    }
};

int test(ListNode n0, ListNode n1) {
    Solution sln = Solution();
    ListNode *n2 = sln.addTwoNumbers(&n0, &n1);

    n0.show(); printf(" + "); n1.show(); printf(" =\n");
    n2->show(); printf("\n");

    delete n2;
    return 0;
}

int main() {
    test(ListNode(0), ListNode(0));
    test(ListNode(3), ListNode(4));
    test(ListNode(3), ListNode(7));
    test(ListNode(3), ListNode(99));
    test(ListNode(342), ListNode(465));
    test(ListNode(3421234), ListNode(46587897));
    return 0;
}
