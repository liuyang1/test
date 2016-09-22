#include <sys/time.h>
#include <unistd.h>
#include "leet.h"

int countNodes(struct TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

#define CASE(a, e) {struct TreeNode *t = buildTree(a, sizeof(a) / sizeof(int)); \
                    int n = countNodes(t);                                      \
                    printf("n=%d ?= e=%d %s\n", n, e, expect(n == e)); }
int basicTest() {
    CASE(((int[]) {}), 0);
    CASE(((int[]) {1}), 1);
    CASE(((int[]) {1, 2, 3, 4}), 4);
    return 0;
}

#define TIC {                       \
        struct timeval start, stop; \
        gettimeofday(&start, NULL);

#define TOC gettimeofday(&stop, NULL);                                \
    printf("%lu sec %luusec\n",                                       \
           stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec); \
    }

int perfTest() {
#define LEN 10000
    int *a = malloc(sizeof(int) * LEN);
    bzero(a, sizeof(int) * LEN);

    TIC;
    struct TreeNode *t = buildTree(a, LEN);
    int n = countNodes(t);
    printf("n=%d ?= e=%d %s\n", n, LEN, expect(n == LEN));
    TOC;

    free(a);
}

int main() {
    basicTest();
    perfTest();
    return 0;
}
