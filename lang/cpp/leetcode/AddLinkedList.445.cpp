#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;

static const int base = 10;
/**
 * Definition for singly-linked list.
 * Most-significant digit come first
 * 7->2->4->3 + 5->6->4
 * 7->8->0->7
 *
 * DO NOT change input list, DO NOT reverse input list
 *
 * With array helper
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
            return;
        }
        next = new ListNode(x / base);
        val = x % base;
    }
    ListNode(int x, ListNode *next) : val(x), next(next) {}
    ~ListNode() {
        if (next) {
            delete next;
        }
    }
    void show(char trail='\0') {
        printf("%d", val);
        if (next != NULL) {
            next->show();
        }
        if (trail != '\0') {
            putchar(trail);
        }
    }
};

class Solution {
    int length(ListNode *l) {
        if (l->val == 0) {
            return 0;
        }
        ListNode *p = l;
        int len;
        for (len = 0; p != NULL; p = p->next, len++) {
        }
        return len;
    }
    int *toArray(ListNode *l, int len, int padlen) {
        ListNode *p;
        int *xs = new int[padlen];
        int i;
        for (i = 0; i != padlen - len; i++) {
            xs[i] = 0;
        }
        for (p = l; i != padlen; i++, p = p->next) {
            xs[i] = p->val;
        }
        return xs;
    }
    ListNode *fromArray(int *a, int len) {
        ListNode *r = NULL;
        int i;
        for (i = len - 1; i >= 0; i--) {
            r = new ListNode(a[i], r);
        }
        if (r->val == 0 && r->next == NULL) {
            return r;
        }
        ListNode *h = r, *last = NULL;
        for (; r->val == 0; ) {
            last = r;
            r = r->next;
        }
        if (last != NULL) {
            last->next = NULL;
            delete h;
        }
        return r;
    }
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            int len1 = length(l1), len2 = length(l2);
            int mlen = max(len1, len2) + 1;

            int *xs1 = toArray(l1, len1, mlen);
            int *xs2 = toArray(l2, len2, mlen);
            int i, carry = 0;
            for (i = mlen - 1; i >= 0; i--) {
                carry = xs1[i] + xs2[i] + carry;
                xs1[i] = carry % base, carry /= base;
            }

            ListNode *r = fromArray(xs1, mlen);
            delete[] xs1;
            delete[] xs2;
            return r;
        }
};

int test(ListNode n0, ListNode n1) {
    n0.show(); printf(" + "); n1.show(); printf(" =\n");

    Solution sln = Solution();
    ListNode *n2 = sln.addTwoNumbers(&n0, &n1);
    if (n2 == NULL) {
        return -1;
    }

    n2->show(); printf("\n");

    delete n2;
    return 0;
}

int test_one() {
    ListNode n = ListNode(123456);
    n.show('\n');
    return 0;
}

int main() {
    // test_one();
    test(ListNode(0), ListNode(0));
    test(ListNode(3), ListNode(4));
    test(ListNode(3), ListNode(7));
    test(ListNode(3), ListNode(99));
    test(ListNode(342), ListNode(465));
    test(ListNode(3421234), ListNode(46587897));
    return 0;
}
