#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int (*CmpFun)(const void *, const void *);

int cmp_int(const void *a, const void *b) {
    const int *pa = a;
    const int *pb = b;
    return *pa - *pb;
}

void show_int_arr(unsigned int *base, size_t nmemb) {
    size_t i;
    for (i = 0; i != nmemb; i++) {
        printf("%u ", base[i]);
    }
    printf("\n");
}

/** Binary Tree with quick sort
 * ref:
 *https://tekmarathon.com/2013/09/20/analogy-between-binary-search-tree-and-quicksort-algorithm/
 */

typedef struct Node {
    void *val;
    struct Node *left;
    struct Node *right;
} Node;

static void show_int_bin_tree_in(Node *n, int level) {
    if (n == NULL) {
        return;
    }
    int *p = n->val;
    show_int_bin_tree_in(n->left, level + 1);
    size_t i;
    for (i = 0; i != level; i++) {
        printf("    ");
    }
    printf("%d\n", *p);
    show_int_bin_tree_in(n->right, level + 1);
}

void show_int_bin_tree(Node *n) {
    show_int_bin_tree_in(n, 0);
}

static void assign(void *a, void *b, size_t size) {
    memcpy(a, b, size);
}

void free_bin_tree(Node *n) {
    if (n == NULL) {
        return;
    }
    free(n->val);
    free_bin_tree(n->left);
    free_bin_tree(n->right);
    free(n);
}

static Node *qsort_bintree_in(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    char *p = base;
    if (nmemb == 0) {
        return NULL;
    }
    Node *n = malloc(sizeof(Node));
    n->val = malloc(size);
    assign(n->val, p, size);
    p += size;
    nmemb -= 1;
    if (nmemb == 0) {
        n->left = n->right = NULL;
        return n;
    }
    char *l = malloc(size * nmemb), *lc = l;
    char *r = malloc(size * nmemb), *rc = r;
    size_t i;
    for (i = 0; i != nmemb; i++, p += size) {
        if (cmp(p, n->val) <= 0) {
            assign(lc, p, size);
            lc += size;
        } else {
            assign(rc, p, size);
            rc += size;
        }
    }
    n->left = qsort_bintree_in(l, (lc - l) / size, size, cmp);
    n->right = qsort_bintree_in(r, (rc - r) / size, size, cmp);
    free(l);
    free(r);
    return n;
}

// midfix to traversal binary tree
static int bintree_to_arr(Node *n, void *base, size_t nmemb, size_t size) {
    if (n == NULL) {
        return 0;
    }
    int r = 0;
    r += bintree_to_arr(n->left, base + r * size, nmemb, size);
    assign(base + r * size, n->val, size);
    r++;
    r += bintree_to_arr(n->right, base + r * size, nmemb - r, size);
    assert(nmemb >= r);
    return r;
}

void qsort_bintree(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    Node *root = qsort_bintree_in(base, nmemb, size, cmp);
    // show_int_bin_tree(root); // for debug
    bintree_to_arr(root, base, nmemb, size);
    free_bin_tree(root);
}

/** Radix Sort
 * ref: https://en.wikipedia.org/wiki/Radix_sort
 */
typedef unsigned int ValT;
const size_t ValBits = 32;

void radix_sort(ValT *p, size_t num) {
    size_t i, j, m, n;
    ValT mask;
    ValT *buck0 = malloc(sizeof(ValT) * num);
    ValT *buck1 = malloc(sizeof(ValT) * num);
    for (i = 0, mask = 1; i != ValBits; i++, mask = mask << 1) {
        for (j = m = n = 0; j != num; j++) {
            if ((p[j] & mask) == 0) {
                buck0[m++] = p[j];
            } else {
                buck1[n++] = p[j];
            }
        }
        assert(m + n == num);
        if (m == 0 || n == 0) {
            break;
        }
        for (j = 0; j != m; j++) {
            p[j] = buck0[j];
        }
        for (; j != num; j++) {
            p[j] = buck1[j - m];
        }
    }
    free(buck0);
    free(buck1);
}

int main() {
    unsigned int a[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    size_t n = sizeof(a) / sizeof(a[0]);
    show_int_arr(a, n);
    // qsort_bintree(a, n, sizeof(a[0]), cmp_int);
    radix_sort(a, n);
    show_int_arr(a, n);
    return 0;
}
