#include "leet.h"

#include "mergeList.021.c"

struct ListNode *mergeKLists(struct ListNode **lists, int listsSize) {
    if (listsSize == 0 || lists == NULL) {
        return NULL;
    }
    if (listsSize == 1) {
        return lists[0];
    }
    if (listsSize == 2) {
        return mergeTwoLists(lists[0], lists[1]);
    }
    struct ListNode *a = mergeKLists(lists, listsSize / 2);
    struct ListNode *b = mergeKLists(lists + listsSize / 2, listsSize - listsSize / 2);
    return mergeTwoLists(a, b);
}

bool testMergeKLists() {
    int a[][4] = {
        {0, 1, 4, 7},
        {2, 3, 4, 8},
        {1, 4, 7, 9}
    };
    int n = COUNT_OF(a);
    struct ListNode **lists = malloc(sizeof(struct ListNode *) * n);
    int i;
    for (i = 0; i != n; i++) {
        lists[i] = buildLst(a[i], COUNT_OF(a[i]));
    }
    struct ListNode *r = mergeKLists(lists, n);
    showLst(r);
    return true;
}

int main() {
    testMergeKLists();
    return 0;
}
