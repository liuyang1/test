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

/** odd-even sort
 *  It like bubble sort,
 *  but check odd-even this time, and switch to even-odd next time
 *  so we could spwan pairs to parallel work
 *  To simplify code for parallel process, we remove "HEAD/TAIL" optimzation
 */
void oddeven_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, j;
    void *t = malloc(size);
    for (i = 0; i != nmemb; i++) {
        for (j = i % 2; j < nmemb - 1; j += 2) {
            if (cmp(base + j * size, base + (j + 1) * size) > 0) {
                swap(base + j * size, base + (j + 1) * size, size, t);
            }
        }
        show_int_arr(base, nmemb);
    }
    free(t);
}

static const int PNUM = 4;
#include <pthread.h>
typedef struct {
    void *base;
    size_t nmemb;
    size_t size;
    CmpFun cmp;
    size_t times;
    void *t;
} Arg;
static void oddeven_routine_in(void *base, size_t nmemb, size_t size,
                               CmpFun cmp, void *t) {
    size_t i;
    for (i = 0; i < nmemb - 1; i += 2) {
        if (cmp(base + i * size, base + (i + 1) * size) > 0) {
            swap(base + i * size, base + (i + 1) * size, size, t);
        }
    }
    // show_int_arr(base, nmemb);
}

void *oddeven_routine(void *arg) {
    Arg *p = arg;
    oddeven_routine_in(p->base, p->nmemb, p->size, p->cmp, p->t);
    return NULL;
}

void oddevenP_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, j;
    pthread_t aTh[PNUM];
    Arg *arg = malloc(sizeof(Arg) * PNUM);
    for (j = 0; j != PNUM; j++) {
        arg[j].size = size;
        arg[j].cmp = cmp;
        arg[j].base = base + nmemb * j / PNUM * size;
        arg[j].nmemb = nmemb / PNUM;
        arg[j].t = malloc(size);
    }
    arg[PNUM - 1].nmemb = nmemb - (PNUM - 1) * nmemb / PNUM;
    for (i = 0; i != nmemb; i++) {
        for (j = 0; j != PNUM; j++) {
            arg[j].times = i;
            pthread_create(&aTh[j], NULL, oddeven_routine, arg + j);
        }
        for (j = 0; j != PNUM; j++) {
            pthread_join(aTh[j], NULL);
            arg[j].base += (i % 2 == 0 ? 1 : -1) * size;
        }
        arg[PNUM - 1].nmemb += i % 2 == 0 ? -1 : 1;
        // show_int_arr(base, nmemb);
    }
    for (j = 0; j != PNUM; j++) {
        free(arg[j].t);
    }
    free(arg);
}

/** comb sort
 *  bubble sort's gap is always 1;
 *  comb sort's gap is shrink from nmemb/k to 1,
 *  This will accelerate turtule's speed.
 */
static size_t shrink(size_t gap) {
    if (gap == 1) {
        return 1;
    } else {
        return gap / 1.3;
    }
}

void comb_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    size_t i, j;
    void *t = malloc(size);
    size_t gap = shrink(nmemb);
    for (i = 0; i != nmemb; i++, gap = shrink(gap)) {
        bool swaped = false;
        for (j = 0; j + gap != nmemb; j++) {
            if (cmp(base + j * size, base + (j + gap) * size) > 0) {
                swap(base + j * size, base + (j + gap) * size, size, t);
                swaped = true;
            }
        }
        if (gap == 1 && swaped == false) {
            break;
        }
        // show_int_arr(base, nmemb);
    }
    free(t);
}

/** gnome sort
 *  after optimization, it's variant of insert sort
 *  just waster ASSIGN operation in SWAP
 */
void gnome_sort_in(void *base, size_t nmemb, size_t size, CmpFun cmp, void *t) {
    size_t i;
    for (i = nmemb;
         i > 0 && cmp(base + (i - 1) * size, base + i * size) > 0;
         i--) {
        swap(base + i * size, base + (i - 1) * size, size, t);
    }
}

void gnomeO_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    void *t = malloc(size);
    size_t i;
    for (i = 1; i != nmemb; i++) {
        gnome_sort_in(base, i, size, cmp, t);
        show_int_arr(base, nmemb);
    }
    free(t);
}

/** gnome sort
 *  single loop
 *  But it waste too many times to move back to TELEPORT
 */
void gnome_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    void *t = malloc(size);
    size_t i = 0;
    while (i < nmemb) {
        if (i == 0 || cmp(base + i * size, base + (i - 1) * size) >= 0) {
            i++;
        } else {
            swap(base + i * size, base + (i - 1) * size, size, t);
            i--;
        }
    }
    free(t);
}

/** Insert Sort
 *  sort card when play bridge card
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

typedef struct ListNd {
    void *val;
    struct ListNd *next;
} ListNd;
/** insert sort with linked list
 *  Yes, it's quick when insert data;
 *  However, linked list cannot be random accessed,
 *  so we cannot binary search on it
 *  This algo is O(N^2), too.
 */

static ListNd *create_listnd(void *val) {
    ListNd *n = malloc(sizeof(ListNd));
    n->val = val;
    n->next = NULL;
    return n;
}

static void show_list(ListNd *h) {
    for (; h != NULL; h = h->next) {
        int *p = h->val;
        printf("%d ", *p);
    }
    printf("\n");
}

static void destory_list(ListNd *h) {
    ListNd *t;
    while (h != NULL) {
        t = h->next;
        free(h);
        h = t;
    }
}

void insertL_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    if (nmemb <= 1) {
        return;
    }
    void *p = malloc(size * nmemb);
    memcpy(p, base, size * nmemb);
    size_t i;
    ListNd *h = create_listnd(p), *c;
    for (i = 1; i != nmemb; i++) {
        ListNd *n = create_listnd(p + i * size);
        ListNd *prev = n;
        for (c = h; c != NULL && cmp(n->val, c->val) < 0; prev = c, c = c->next) {
        }
        if (prev == n) {
            n->next = h;
            h = n;
        } else {
            prev->next = n;
            n->next = c;
        }
        // show_list(h); // reversed order list
    }
    for (i = nmemb, c = h; i != 0; i--, c = c->next) {
        assign(base + (i - 1) * size, c->val, size);
    }
    destory_list(h);
    free(p);
}

/** insert sort with skip list
 *  with skip list struct, it could reach O(NlogN)
 */
// insertS_sort

/** slow sort
 */
void slow_sort_in(void *base, size_t i, size_t j, size_t size,
                  CmpFun cmp, void *t) {
    if (i >= j) {
        return;
    }
    size_t m = (i + j) / 2;
    slow_sort_in(base, i, m, size, cmp, t);
    slow_sort_in(base, m + 1, j, size, cmp, t);
    if (cmp(base + j * size, base + m * size) < 0) {
        swap(base + j * size, base + m * size, size, t);
    }
    slow_sort_in(base, i, j - 1, size, cmp, t);
    // show_int_arr(base + i * size, j - i + 1);
    show_int_arr(base, 16);
}

void slow_sort(void *base, size_t nmemb, size_t size, CmpFun cmp) {
    void *t = malloc(size);
    slow_sort_in(base, 0, nmemb - 1, size, cmp, t);
    free(t);
}
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

typedef struct TreeNd {
    void *val;
    struct TreeNd *left;
    struct TreeNd *right;
} TreeNd;

static void show_int_bin_tree_in(TreeNd *n, int level) {
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

void show_int_bin_tree(TreeNd *n) {
    show_int_bin_tree_in(n, 0);
}

void free_bin_tree(TreeNd *n) {
    if (n == NULL) {
        return;
    }
    free(n->val);
    free_bin_tree(n->left);
    free_bin_tree(n->right);
    free(n);
}

static TreeNd *qsort_bintree_in(void *base, size_t nmemb, size_t size,
                                CmpFun cmp) {
    char *p = base;
    if (nmemb == 0) {
        return NULL;
    }
    TreeNd *n = malloc(sizeof(TreeNd));
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
static int bintree_to_arr(TreeNd *n, void *base, size_t nmemb, size_t size) {
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
    TreeNd *root = qsort_bintree_in(base, nmemb, size, cmp);
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
    // oddeven_sort(a, n, sz, cmp_int);
    // oddevenP_sort(a, n, sz, cmp_int);
    // comb_sort(a, n, sz, cmp_int);
    // gnome_sort(a, n, sz, cmp_int);
    // insert_sort(a, n, sizeof(a[0]), cmp_int);
    // insertR_sort(a, n, sizeof(a[0]), cmp_int);
    // insertB_sort(a, n, sz, cmp_int);
    // insertL_sort(a, n, sz, cmp_int);
    slow_sort(a, n, sz, cmp_int);
    // merge_sort(a, n, sizeof(a[0]), cmp_int);
    printf("gCmpCnt=%d gAssignCnt=%d\n", gCmpCnt, gAssignCnt);
    show_int_arr(a, n);
    return 0;
}
