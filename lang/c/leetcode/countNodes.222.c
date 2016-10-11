#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include "leet.h"

int leftheight(struct TreeNode *root) {
    int h;
    for (h = 0; root; root = root->left) {
        h++;
    }
    return h;
}

int rightheight(struct TreeNode *root) {
    int h;
    for (h = 0; root; root = root->right) {
        h++;
    }
    return h;
}

/** complexity: O(h ^ 2) = O(logN ^ 2)
 * T(h) = 2h                if complete binary tree
 *      = 2(h-1) + T(h-1)   if left complete, but right isnot
 *      = T(h-1)            if left isnot complete
 * so upper bound:
 * Upper(T(h)) = sum(2*(i - 1))    1 <= i <= h
 *             = O(h ^ 2)
 */
int countNodes(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int lh = leftheight(root);
    int rh = rightheight(root);
    if (lh == rh) {
        return (1 << rh) - 1;
    } else {
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
}

int countNodes_naive(struct TreeNode *root) {
    if (root == 0) {
        return 0;
    }
    return 1 + countNodes_naive(root->left) + countNodes_naive(root->right);
}

#define CASE(a, e) {struct TreeNode *t = buildTree(a, sizeof(a) / sizeof(int)); \
                    int n = countFn(t);                                         \
                    printf("n=%d ?= e=%d %s\n", n, e, expect(n == e)); }
bool basicTest(int(countFn)(struct TreeNode *)) {
    CASE(((int[]) {}), 0);
    CASE(((int[]) {1}), 1);
    CASE(((int[]) {1, 2, 3, 4}), 4);
    return true;
}

#define LEN (1000 * 1000 * 10)
bool perfTest(int(countFn)(struct TreeNode *)) {
    bool ret = true;
    int *a = malloc(sizeof(int) * LEN);
    bzero(a, sizeof(int) * LEN);

    struct TreeNode *t = buildTree(a, LEN);
    int n;

    struct timeval start, stop;
    gettimeofday(&start, NULL);
    {
        n = countFn(t);
    }
    gettimeofday(&stop, NULL);

    printf("n=%d ?= e=%d %s\n", n, LEN, expect(n == LEN));

    unsigned long dt = stop.tv_usec - start.tv_usec;
    printf("%lu sec %luusec\n", stop.tv_sec - start.tv_sec, dt);
    if (dt > 200) {
        ret = false;
    }
    printf("%s: %s\n", __func__, expect(ret));

    free(a);
    return ret;
}

#define TEST(fn) {printf("---- test on func %s ----\n", # fn); test(fn); }
bool test(int(countFn)(struct TreeNode *)) {
    return basicTest(countFn) && perfTest(countFn);
}

int main() {
    TEST(countNodes_naive);
    TEST(countNodes);
    return 0;
}
