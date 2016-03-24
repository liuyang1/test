#include "leet.h"

typedef struct {
    int *buf;
    int len;
    int cap;
} ostream;

ostream *ostream_reset(ostream *os) {
    os->buf = NULL;
    os->len = 0;
    os->cap = 0;
}

void ostream_chkput(ostream *os, int size) {
    size += os->len;
    int *p;
    if (size >= os->cap) {
        p = realloc(os->buf, sizeof(int) * size);
        if (p == NULL) {
            free(os->buf);
            printf("fatal error, no memory\n");
        }
        os->buf = p;
        os->cap = size;
    }
}

ostream *ostream_create() {
    ostream *os = malloc(sizeof(ostream));
    ostream_reset(os);
    return os;
}

void ostream_destory(ostream *os) {
    if (os->buf) {
        free(os->buf);
    }
    free(os);
}

void ostream_put(ostream *os, int v) {
    ostream_chkput(os, 1);
    os->buf[os->len] = v;
    os->len++;
}

int *ostream_drain(ostream *os, int *returnSize) {
    if (returnSize != NULL) {
        *returnSize = os->len;
    }
    int *r = os->buf;
    ostream_reset(os);
    return r;
}

void postorderTraversalI(ostream *os, struct TreeNode *root) {
    if (root == NULL) {
        return;
    }
    postorderTraversalI(os, root->left);
    postorderTraversalI(os, root->right);
    ostream_put(os, root->val);
}

int *postorderTraversal(struct TreeNode *root, int *returnSize) {
    ostream *os = ostream_create();
    postorderTraversalI(os, root);
    int *r = ostream_drain(os, returnSize);
    ostream_destory(os);
    return r;
}

#define CASE(a) {struct TreeNode *t = buildTree(a, sizeof(a) / sizeof(int)); \
                 int sz, *r = postorderTraversal(t, &sz); showArr(r, sz); free(r); }
int main() {
    CASE(((int[]) {}));
    CASE(((int[]) {1, NIL, 3}));
    CASE(((int[]) {1, 2, 3, 4, 5, 6}));
}
