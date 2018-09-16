#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef int (*CmpFun)(const void *, const void *);
typedef void (*SortFun)(void *base, size_t nmemb, size_t size, CmpFun cmp);

unsigned int gCmpCnt = 0;
unsigned int gAssignCnt = 0;
int cmp_int(const void *a, const void *b) {
    gCmpCnt++;
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

static void assign(void *a, void *b, size_t size) {
    assert(a != NULL && b != NULL && size != 0 && a != b);
    gAssignCnt++;
    memcpy(a, b, size);
}

static void swap(void *a, void *b,  size_t size, void *t) {
    assign(t, a, size);
    assign(a, b, size);
    assign(b, t, size);
}

/** always pick the minimum one, then swap it to head
 */
void select_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, j, midx;
    void *t = malloc(size);
    int cnt = 0;
    for (i = 0; i != nmemb - 1; i++) {
        for (midx = i, j = i + 1; j != nmemb; j++) {
            if (cmp(base + midx * size, base + j * size) > 0) {
                midx = j;
            }
            cnt++;
        }
        if ( i != midx ) {
            swap(base + i * size, base + midx * size, size, t);
        }
    }
    printf("%d\n", cnt);
    free(t);
}

/** check pair every time, reorder it
 * This will floating one biggest bubble to END every time
 * so we could check the END position to saving useless loop
 * - This big one is rabbit
 * - This small one is turtle :)
 * so it should naming to sinking sort instead of bubble sort :)
 */
void bubble_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, j, tail, ntail;
    void *t = malloc(size);
    for (i = 0, tail = nmemb - 1; i != nmemb; i++) {
        for (j = 0; j != tail; j++) {
            if (cmp(base + j * size, base + (j + 1) * size) > 0) {
                swap(base + j * size, base + (j + 1) * size, size, t);
                ntail = j;
            }
        }
        if (tail == ntail) {
            break;
        }
        tail = ntail;
    }
    free(t);
}

/** Cocktail Shaker Sort or Bidirectional Bubble Sort
 *  It check from head to tail, then back from tail to head (Bidirectional)
 *  so it could save time as bubble sorting's optimizing skill.
 */
void cocktail_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, j, head, nhead, tail, ntail;
    void *t = malloc(size);
    for (i = head = 0, tail = nmemb - 1; i != nmemb; i++) {
#if 1
        for (j = head; j != tail; j++) {
            if (cmp(base + j * size, base + (j + 1) * size) > 0) {
                swap(base + j * size, base + (j + 1) * size, size, t);
                ntail = j;
            }
        }
        if (tail == ntail) {
            break;
        }
        tail = ntail;
        printf("head=%d tail=%d cnt=%d\n", head, tail, gCmpCnt);
        show_int_arr(base, nmemb);
#endif
        for (j = tail; j != head; j--) {
            if (cmp(base + j * size, base + (j - 1) * size) < 0) {
                swap(base + j * size, base + (j - 1) * size, size, t);
                nhead = j;
            }
        }
        if (head == nhead) {
            break;
        }
        head = nhead;
        printf("head=%d tail=%d cnt=%d\n", head, tail, gCmpCnt);
        show_int_arr(base, nmemb);
    }

    free(t);
}

/** Insert Sort
 *  sotr card when play bridge card
 */
void insert_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    if (nmemb <= 1) {
        return;
    }
    size_t i, k;
    void *t = malloc(size);
    for (i = 1; i != nmemb; i++) {
#if 1
        // search correct position from tail
        assign(t, base + i * size, size);
        for (k = i - 1; k >= 0 && cmp(base + k * size, t) > 0; k--) {
            assign(base + (k + 1) * size, base + k * size, size);
        }
        assign(base + (k + 1) * size, t, size);
#else
        size_t j;
        // This is WRONG, as waster too many checking times
        // search correct position from head
        for (j = 0; j != i && cmp(base + j * size, base + i * size) <= 0; j++) {
        }
        // insert
        assign(t, base + i * size, size);
        for (k = i; k != j; k--) {
            assign(base + k * size, base + (k - 1) * size, size);
        }
        assign(base + j * size, t, size);
#endif
        show_int_arr(base, nmemb);
    }
}

void insertR_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    if (nmemb <= 1) {
        return;
    }
    insertR_sort(base + size, nmemb - 1, size, cmp);
    void *t = malloc(size);
    assign(t, base, size);
    size_t i;
    for (i = 1; i != nmemb && cmp(t, base + i * size) > 0; i++) {
        assign(base + (i - 1) * size, base + i * size, size);
    }
    assign(base + (i - 1) * size, t, size);
    // show_int_arr(base, nmemb);
    free(t);
}

/** Binary insert sort
 *  It only save time for search correct position,
 *  but whole algo is O(N^2) as we still need slow shift operation
 */
void insertB_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, k, b, e, m;
    void *t = malloc(size);
    for (i = 1; i != nmemb; i++) {
        assign(t, base + i * size, size);
        for (b = 0, e = i; b <= e;) {
            m = (b + e) / 2;
            if (cmp(t, base + m * size) < 0) {
                e = m - 1;
            } else {
                b = m + 1;
            }
        }
        m = (b + e) / 2;
        if (m != i) {
            for (k = i - 1; k > m; k--) {
                assign(base + (k + 1) * size, base + k * size, size);
            }
            assign(base + (m + 1) * size, t, size);
        }
    }
    free(t);
}

/** insert sort with linked list
 *  Yes, it's quick when insert data;
 *  However, linked list cannot be random accessed,
 *  so we cannot binary search on it
 *  This algo is O(N^2), too.
 */
// insertL_sort

/** insert sort with skip list
 *  with skip list struct, it could reach O(NlogN)
 */
// insertS_sort
/** Merge sort
 *  when merge to two sub sequences, we need extra storage.
 */
void merge_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    if (nmemb <= 1) {
        return;
    }
    size_t mid = nmemb / 2;
    merge_sort(base, mid, size, cmp);
    merge_sort(base + mid * size, nmemb - mid, size, cmp);
    void *t = malloc(size * nmemb);
    size_t i, j, k, idx;
    for (k = i = 0, j = mid; i != mid && j != nmemb; k++) {
        if (cmp(base + i * size, base + j * size) <= 0) {
            idx = i++;
        } else {
            idx = j++;
        }
        assign(t + k * size, base + idx * size, size);
    }
    for (; i != mid; i++, k++) {
        assign(t + k * size, base + i * size, size);
    }
    for (; j != nmemb; j++, k++) {
        assign(t + k * size, base + j * size, size);
    }
    assert(k == nmemb);
    memcpy(base, t, size * nmemb);
    free(t);
}

/** Binary Tree with quick sort
 * ref:
 * https://tekmarathon.com/2013/09/20/analogy-between-binary-search-tree-and-quicksort-algorithm/
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

void free_bin_tree(Node *n) {
    if (n == NULL) {
        return;
    }
    free(n->val);
    free_bin_tree(n->left);
    free_bin_tree(n->right);
    free(n);
}

static Node *qsort_bintree_in(void *base, size_t nmemb, size_t size,
                              CmpFun cmp) {
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
        /** End when sorted, skip useless checking */
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

size_t inversion(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t cnt = 0;
    size_t i, j;
    for (i = 0; i != nmemb - 1; i++) {
        for (j = i + 1; j != nmemb; j++) {
            cnt += cmp(base + i * size, base + j * size) > 0;
        }
    }
    return cnt;
}

double inversion_ratio(size_t cnt, size_t nmemb) {
    return 2. * cnt / (nmemb + 0.) / (nmemb - 1.);
}

int main() {
    unsigned int a[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    size_t n = sizeof(a) / sizeof(a[0]);
    size_t sz = sizeof(a[0]);
    show_int_arr(a, n);
    // double ratio = inversion_ratio(inversion(a, n, sizeof(a[0]), cmp_int),
    //                                n);
    // printf("inversion ratio=%f\n", ratio);
    // qsort_bintree(a, n, sizeof(a[0]), cmp_int);
    // radix_sort(a, n);
    // select_sort(a, n, sizeof(a[0]), cmp_int);
    // bubble_sort(a, n, sizeof(a[0]), cmp_int);
    // cocktail_sort(a, n, sizeof(a[0]), cmp_int);
    // insert_sort(a, n, sizeof(a[0]), cmp_int);
    // insertR_sort(a, n, sizeof(a[0]), cmp_int);
    insertB_sort(a, n, sz, cmp_int);
    // merge_sort(a, n, sizeof(a[0]), cmp_int);
    printf("gCmpCnt=%d gAssignCnt=%d\n", gCmpCnt, gAssignCnt);
    show_int_arr(a, n);
    return 0;
}
