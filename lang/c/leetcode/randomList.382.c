#include "leet.h"

typedef struct {
    struct ListNode *head;
} Solution;

/** @param head The linked list's head.
 *      Note that the head is guaranteed to be not null,
 *      so it contains at least one node.
 */
Solution *solutionCreate(struct ListNode *head) {
    Solution *sln = malloc(sizeof(*sln));
    sln->head = head;
    return sln;
}

/** Returns a random node's value. */
int solutionGetRandom(Solution *sln) {
    int x = sln->head->val;
    int l = 1;
    struct ListNode *n = sln->head;
    while (n = n->next) {
        // just pick Nth one, if p == 1 / N
        l++;
        long int v = random() % l;
        if (v == 0) {
            x = n->val;
        }
    }
    return x;
}

void solutionFree(Solution *sln) {
    free(sln);
}

/**
 * Your Solution struct will be instantiated and called as such:
 * struct Solution* obj = solutionCreate(head);
 * int param_1 = solutionGetRandom(obj);
 * solutionFree(obj);
 */

int test(int times) {
#define LEN 10
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct ListNode *head = buildLst(a, COUNT_OF(a));
    Solution *sln = solutionCreate(head);
    int cnt[LEN] = {0};
    int i;
    for (i = 0; i != times; i++) {
        int x = solutionGetRandom(sln);
        cnt[x]++;
    }
    solutionFree(sln);
    freeLst(head);

    showArr(cnt, LEN);
    int d;
    for (i = 0; i != LEN; i++) {
        int dd = cnt[i] - (times / LEN);
        d += dd > 0 ? dd : (-dd);
    }
    printf("diff = %d / %d = %.6f\n", d, times, (d + 0.0) / times);
    return 0;
}

int main() {
    int i, t, base = 10;
    for (i = 0, t = base; i != 7; i++, t *= base) {
        test(t);
    }
    return 0;
}
