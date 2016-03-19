#include "leet.h"
/**
 * @brief ostream: OutputStream
 * put any data to stream, finally drain from it.
 * It work like StringBuffer in Java.
 *
 * API:
 * create               create one ostream with unit length
 * destroy              destory one stream
 * length               return number of already output units
 * put                  put one unit data to stream
 * puts                 put N units data to stream
 * trim                 remove last N units data
 * drain                return all data in stream, and clear stream
 *                      returned data, need free
 * copy                 copy stream and data in it
 *                      data and stream need free
 *
 * Internal function:
 * reset                reset status in stream, lose data
 * chkput               check output buffer size, if not enough, enlarge it.
 */
typedef struct {
    char *buf;
    int unit;
    int nmem;
    int cap;
} ostream;
void ostream_reset(ostream *os) {
    os->nmem = 0;
    os->cap = 0;
    os->buf = NULL;
}

ostream *ostream_create(int unit) {
    ostream *os = malloc(sizeof(ostream));
    os->unit = unit;
    ostream_reset(os);
    return os;
}

void ostream_destory(ostream *os) {
    if (os->buf) {
        free(os->buf);
    }
    free(os);
}

void ostream_chkput(ostream *os, int nmem) {
    int demand_size = os->nmem + nmem;
    if (demand_size > os->cap) {
        if (os->cap == 0) {
            os->cap = 1;
        }
        do {
            os->cap *= 2;
        } while (demand_size > os->cap);
        reallocM(&os->buf, os->unit * os->cap);
    }
}

void ostream_puts(ostream *os, void *data, int nmem) {
    ostream_chkput(os, nmem);
    memcpy(os->buf + os->unit * os->nmem, data, os->unit * nmem);
    os->nmem += nmem;
}

void ostream_put(ostream *os, void *data) {
    ostream_puts(os, data, 1);
}

void ostream_trim(ostream *os, int nmem) {
    os->nmem -= nmem;
}

int ostream_length(ostream *os) {
    return os->nmem;
}

void *ostream_drain(ostream *os) {
    void *r = os->buf;
    ostream_reset(os);
    return r;
}

ostream *ostream_copy(ostream *os) {
    ostream *n = ostream_create(os->unit);
    ostream_puts(n, os->buf, os->nmem);
    return n;
}

// ostream test code
void test_os_char() {
    ostream *os = ostream_create(sizeof(char));
    char s0[] = "hello";
    char c1 = ' ';
    char s2[] = "world";
    ostream_puts(os, s0, strlen(s0));
    ostream_put(os, &c1);
    ostream_puts(os, s2, strlen(s2) + 1);
    char *p = ostream_drain(os);
    printf("%s\n", p);
    ostream_destory(os);
    free(p);
}

void test_os_ptr() {
    ostream *os = ostream_create(sizeof(char *));
    char *p0 = "hello";
    char *p1 = "world";
    ostream_put(os, &p0);
    ostream *os1 = ostream_copy(os);
    ostream_put(os, &p1);
    char **pp = ostream_drain(os);
    printf("%s %s\n", pp[0], pp[1]);

    char **pp1 = ostream_drain(os1);
    printf("%s\n", pp1[0]);

    ostream_destory(os1);
    ostream_destory(os);
    free(pp);
    free(pp1);
}

int test_os() {
    test_os_char();
    test_os_ptr();
    return 0;
}

// ostream DONE

/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#define LEN 16
#define DELIMTER    "->"
void dfs(struct TreeNode *node, ostream *path, ostream *lst) {
    char s[LEN];
    snprintf(s, LEN, "%d%s", node->val, DELIMTER);
    ostream_puts(path, s, strlen(s));

    if (node->left == NULL && node->right == NULL) {
        // remove last delimter
        ostream_trim(path, strlen(DELIMTER));
        // append \0, to make it work like C-string
        char end = '\0';
        ostream_put(path, &end);

        char *final_path = ostream_drain(path);
        ostream_put(lst, &final_path);
        ostream_destory(path);
        return;
    }
    if (node->left && node->right) {
        // duplicate path, and recursive.
        ostream *path1 = ostream_copy(path);
        dfs(node->left, path, lst);
        dfs(node->right, path1, lst);
        return;
    }
    struct TreeNode *next = node->left ? node->left : node->right;
    dfs(next, path, lst);
}

char **binaryTreePaths(struct TreeNode *root, int *returnSize) {
    if (root == NULL) {
        *returnSize = 0;
        return NULL;
    }
    ostream *path = ostream_create(sizeof(char));
    ostream *lst = ostream_create(sizeof(char *));

    dfs(root, path, lst);

    *returnSize = ostream_length(lst);
    char **r = ostream_drain(lst);
    return r;
}

#define CASE(a) {struct TreeNode *t = buildTree(a, sizeof(a) / sizeof(int)); \
                 showTree(t); \
                 int i, sz; char **r = binaryTreePaths(t, &sz); \
                 for (i = 0; i != sz; i++) { \
                     printf("%s\n", r[i]); free(r[i]); } \
                 free(r); }

int main() {
    CASE(((int[]) {}));
    CASE(((int[]) {1, 2}));
    CASE(((int[]) {1, 2, 3}));
    CASE(((int[]) {1, 2, NIL, 3}));
    CASE(((int[]) {1, 2, 3, NIL, 4}));
    CASE(((int[]) {4, 2, 5, 1, 3}));
    return 0;
}
