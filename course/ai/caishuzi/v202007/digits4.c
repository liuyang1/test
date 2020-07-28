/**
 * vec container: insert, iterate, extend(++)
 * permuation
 * build tree
 * sieve, equal class
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define seq_t int
#define CONS_ST(a, b, c, d) (a + (b << 8) + (c << 16) + (d << 24))


/** vector container, */
#define VP_T       void *

typedef struct {
    size_t i, size; // fixed header
    VP_T *pa;
} vec_vp;

static inline void vec_deinit(void *p) {
    vec_vp *t = p;
    free(t->pa);
    free(t);
}

static inline vec_vp *vec_vp_init() {
    vec_vp *p = malloc(sizeof(vec_vp));
    p->i = 0;
    p->size = 16;
    p->pa = malloc(sizeof(VP_T) * p->size);
    return p;
}

static inline int vec_vp_add(vec_vp *p, VP_T ptr) {
    if (p->i + 1 == p->size) {
        p->size *= 2;
        p->pa = realloc(p->pa, sizeof(void *) * p->size);
        assert(p->pa != NULL);
    }
    p->pa[p->i++] = ptr;
    return 0;
}

static inline VP_T vec_vp_idx(vec_vp *p, size_t i) {
    assert(i < p->i);
    return p->pa[i];
}

static inline size_t vec_size(void *p) {
    vec_vp *t = p;
    return t->i;
}

typedef struct {
    size_t i, size;
    seq_t *pa;
} vec_seq;

static inline vec_seq *vec_seq_init() {
    vec_seq *p = malloc(sizeof(vec_seq));
    p->i = 0, p->size = 16;
    p->pa = malloc(sizeof(seq_t) * p->size);
    return p;
}

static inline int vec_seq_add(vec_seq *p, seq_t n) {
    if (p->i + 1 == p->size) {
        p->size *= 2;
        p->pa = realloc(p->pa, sizeof(seq_t) * p->size);
        assert(p->pa != NULL);
    }
    p->pa[p->i++] = n;
    return 0;
}

static inline int vec_seq_append(vec_seq *p, vec_seq *q) {
    if (p->i + q->i >= p->size) {
        p->size = p->i + q->i; // up to 2
        p->pa = realloc(p->pa, sizeof(seq_t) * p->size);
        assert(p->pa != NULL);
    }
    memcpy(&p->pa[p->i], q->pa, sizeof(seq_t) * q->i);
    p->i += q->i;
    return 0;
}

static inline vec_seq *vec_seq_dup(vec_seq *p) {
    vec_seq *n = vec_seq_init();
    vec_seq_append(n, p);
    return n;
}

static inline seq_t vec_seq_idx(vec_seq *p, size_t i) {
    assert(i < p->i);
    return p->pa[i];
}

static inline bool vec_seq_elem(vec_seq *p, seq_t x) {
    size_t i;
    for (i = 0; i != vec_size(p); i++) {
        if (x == vec_seq_idx(p, i)) {
            return true;
        }
    }
    return false;
}

static inline char *show_bool(bool x) {
    return x ? "true" : "false";
}

/** check function ***********************************************************/
#define N 10
#define K 4
/** actually, kind of chk result is 5*6/2-1=14
 * We set to 5*6/2, for mapping easily */
#define CHK_KIND_NUM ((K + 1) * (K + 2) / 2 - 1)

/** a=[0..4], b=[0..4], a<=b
 * (0,0) (0,1) (0,2) (0,3) (0,4)
 *       (1,1) (1,2) (1,3) (1,4)
 *             (2,2) (2,3) (2,4)
 *                   (3,3) -(3,4), this is impossible
 *                         (4,4)
 */
int tbl[][(K + 1)] = {{0, 1, 2, 3, 4},
                      {-1, 5, 6, 7, 8},
                      {-1, -1, 9, 10, 11},
                      {-1, -1, -1, 12, -1},
                      {-1, -1, -1, -1, 13}};
static inline int chk2idx(int a, int b) {
    assert(a <= K);
    assert(b <= K);
    int r = tbl[a][b];
    assert(r != -1);
    return r;
}

static inline void show_chk(size_t *cnt) {
    size_t i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        printf("%zu,", cnt[i]);
    }
    printf("\n");
}

/** seq_t struct, it's int :) ************************************************/

static inline bool is_perm(seq_t n) {
    char *x = (char *)&n;
    size_t i, j;
    for (i = 0; i != K; i++) {
        for (j = i + 1; j != K; j++) {
            if (x[i] == x[j]) {
                return false;
            }
        }
    }
    return true;
}

static inline void show_seq(seq_t n) {
    char *x = (char *)&n;
    printf("%d%d%d%d\n", x[0], x[1], x[2], x[3]);
}

static inline void show_seq_suffix(seq_t n) {
    char *x = (char *)&n;
    printf("%d%d%d%d%c", x[0], x[1], x[2], x[3], ',');
}

static inline void show_seq_vec(vec_seq *v) {
    size_t i;
    for (i = 0; i != vec_size(v); i++) {
        show_seq_suffix(vec_seq_idx(v, i));
    }
    printf("\n");
}

/** this opt is useless, keep original style */
#define OPT 0 // 0, original; 1, loop expand; 2, cached
#if (OPT == 0)
/** input is seq a, b, passing via int to save calling cost */
static inline int chk(seq_t a, seq_t b) {
    char *x = (char *)&a;
    char *y = (char *)&b;
    int i, j;
    int r0, r1;
    for (i = r0 = r1 = 0; i != K; i++) {
        r0 += x[i] == y[i];
        for (j = 0; j != K; j++) {
            r1 += x[i] == y[j];
        }
    }
    // show_seq(a);
    // show_seq(b);
    // printf("r0=%d r1=%d\n", r0, r1);
    int r = chk2idx(r0, r1);
    return r;
}
#endif
#if (OPT==2)
static inline int raw_chk(seq_t a, seq_t b) {
    char *x = (char *)&a;
    char *y = (char *)&b;
    int i, j;
    int r0, r1;
    for (i = r0 = r1 = 0; i != K; i++) {
        r0 += x[i] == y[i];
        for (j = 0; j != K; j++) {
            r1 += x[i] == y[j];
        }
    }
    // show_seq(a);
    // show_seq(b);
    // printf("r0=%d r1=%d\n", r0, r1);
    int r = chk2idx(r0, r1);
    return r;
}
static inline int seq2int(seq_t a) {
    char *x = (char *)&a;
    return x[0] *1000+ x[1] *100+ x[2] *10 + x[3];
}
// cached style
#define UPBOUND (9876+1)
#define BASE 123
#define SEQ_RANGE (UPBOUND-BASE)
int8_t chk_tbl[SEQ_RANGE][SEQ_RANGE];

static inline void chk_init(vec_seq *lst) {
    memset(chk_tbl, 0x0, sizeof(chk_tbl));
    size_t i, j;
    for (i = 0; i != vec_size(lst); i++) {
        for (j = 0; j != vec_size(lst); j++) {
            seq_t x = vec_seq_idx(lst, i);
            seq_t y = vec_seq_idx(lst, j);
            int a = seq2int(x);
            int b = seq2int(y);
            chk_tbl[a - BASE][b-BASE] = raw_chk(x, y);
        }
    }
}
static inline int chk(seq_t a, seq_t b) {
    int a_ = seq2int(a);
    int b_ = seq2int(b);
    return chk_tbl[a_-BASE][b_-BASE];
}
#endif

#if (OPT == 1) // this is useless, compiler will do it
static inline int chk(seq_t a, seq_t b) {
    char *x = (char *)&a;
    char *y = (char *)&b;
    int r0 = 0, r1 = 0;
    r0 += x[0] == y[0];
    r0 += x[1] == y[1];
    r0 += x[2] == y[2];
    r0 += x[3] == y[3];

    r1 += x[0] == y[1];
    r1 += x[0] == y[2];
    r1 += x[0] == y[3];
    r1 += x[1] == y[0];
    r1 += x[1] == y[2];
    r1 += x[1] == y[3];
    r1 += x[2] == y[0];
    r1 += x[2] == y[1];
    r1 += x[2] == y[3];
    r1 += x[3] == y[0];
    r1 += x[3] == y[1];
    r1 += x[3] == y[2];
    r1 += r0;
    int r = chk2idx(r0, r1);
    return r;
}
#endif

/** sieve, sieveg, eqk (equal class) ******************************************/

/** input: [seq], pivot
 *  output: [[seq]], partition with pivot, length is CHK_KIND_NUM
 */
vec_seq **sieve(vec_seq *ss, seq_t pivot) {
    vec_seq **va = malloc(sizeof(vec_seq *) * CHK_KIND_NUM);
    size_t i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        va[i] = vec_seq_init();
        // printf("xxx create va[%zu]=%p\n", i, va[i]);
    }
    for (i = 0; i != vec_size(ss); i++) {
        seq_t si = vec_seq_idx(ss, i);
        int r = chk(si, pivot);
        vec_seq_add(va[r], si);
    }
    return va;
}

void free_sieve(vec_seq **va) {
    size_t i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        vec_deinit(va[i]);
    }
    free(va);
}

/** input: [seq], pivot
 * output: [cnt], partition with pivot, length is CHK_KIND_NUM
 * cnt is length of specific kind check result */
//size_t *sieveg(seq_t *ss, size_t n, seq_t pivot) {
size_t *sieveg(vec_seq *ss, seq_t pivot) {
    size_t *r = malloc(sizeof(size_t) * CHK_KIND_NUM);
    memset(r, 0x00, sizeof(size_t) * CHK_KIND_NUM);
    // show_seq(pivot);
    assert(is_perm(pivot));
    size_t i;
    for (i = 0; i != vec_size(ss); i++) {
        assert(is_perm(vec_seq_idx(ss, i)));
        r[chk(vec_seq_idx(ss, i), pivot)]++;
    }
    return r;
}

void free_sieveg(size_t *r) {
    free(r);
}

double entropy(size_t *cnt) {
    size_t i, s;
    for (i = s = 0; i != CHK_KIND_NUM; i++) {
        s += cnt[i];
    }
    double e = 0;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        if (cnt[i] != 0) {
            double p = cnt[i] / (s + 0.0);
            e += -1 * p * log(p);
        }
    }
    return e;
}

/** compare check result */
int cmpg(size_t *chk_ret0, size_t *chk_ret1) {
    int i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        if (chk_ret0[i] != chk_ret1[i]) {
            return chk_ret0[i] - chk_ret1[i];
        }
    }
    return 0;
}

bool any_eq(vec_vp *p, size_t *g) {
    int j;
    for (j = 0; j != vec_size(p); j++) {
        if (0 == cmpg(g, vec_vp_idx(p, j))) {
            return true;
        }
    }
    return false;
}

/** generate equal class
 * input: [seq], [pivot]
 * output [eqk]
 */
//seq_t *eqk(seq_t *ss, size_t n, seq_t *pivot_lst, size_t pivot_num, int *eqk_num) {
static inline void free_eqkg_g(vec_vp *va1) {
    size_t i;
    for (i = 0; i != vec_size(va1); i++) {
        free_sieveg(vec_vp_idx(va1, i));
    }
    vec_deinit(va1);
}

static inline void free_eqkg(vec_seq *va0) {
    vec_deinit(va0);
}

vec_seq *eqkg(vec_seq *ss, vec_seq *pivots, vec_vp **ovg) {
    /** v0 for eqk, v1 for check result */
    vec_seq *va0 = vec_seq_init();
    vec_vp *va1 = vec_vp_init();
    size_t i;
    for (i = 0; i != vec_size(pivots); i++) {
        seq_t pivot = vec_seq_idx(pivots, i);
        size_t *g = sieveg(ss, pivot);
        if (any_eq(va1, g)) {
            free_sieveg(g);
            continue;
        }
        vec_vp_add(va1, g);
        vec_seq_add(va0, pivot);
    }
    if (ovg != NULL) {
        *ovg = va1;
    } else {
        free_eqkg_g(va1);
    }
    return va0;
}

vec_seq *eqk(vec_seq *ss, vec_seq *pivots) {
    return eqkg(ss, pivots, NULL);
}

/** return concated list of equal class */
vec_seq *eqk2(vec_seq **ss) {
    vec_seq *s = vec_seq_init();
    size_t i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        vec_seq *e = eqk(ss[i], ss[i]);
        vec_seq_append(s, e);
        vec_deinit(e);
    }
    return s;
}

/**
 * build tree
 * 0, eqk: [seq] -> [eqk]
 * 1, select: [seq] -> [eqk] -> select -> pivot
 * 2, sieve: [seq] -> pivot -> [[seq]]
 */
typedef enum {
    LEAF = 0,
    NODE = 1,
} nd_t;

typedef struct tree_t {
    nd_t nd;
    seq_t x;
    struct tree_t *child[CHK_KIND_NUM];
} tree_t;

typedef seq_t (selectFn)(size_t h, vec_seq *ss);

seq_t first(size_t h, vec_seq *ss) {
    assert(vec_size(ss) >= 1);
    return vec_seq_idx(ss, 0);
}

seq_t selMaxE(size_t h, vec_seq *ss) {
    if (h == 0) {
        return CONS_ST(0, 1, 2, 3);
    }
    if (vec_size(ss) == 1) {
        return vec_seq_idx(ss, 0);
    }
    assert(vec_size(ss) != 0);
    vec_vp *eqg = NULL;
    vec_seq *eq = eqkg(ss, ss, &eqg);
    double maxm = -1;
    size_t i, mi = -1;
    for (i = 0; i != vec_size(eq); i++) {
        double m = entropy(vec_vp_idx(eqg, i));
        // printf("i=%zu m=%f\t", i, m);
        // show_chk(vec_vp_idx(eqg, i));
        if (maxm < m) {
            maxm = m, mi = i;
        }
    }
    assert(mi != -1);
    seq_t r = vec_seq_idx(eq, mi);
    free_eqkg_g(eqg);
    free_eqkg(eq);
    return r;
}

size_t height(tree_t *t, size_t h);
tree_t *buildTh1(selectFn f, size_t h, vec_seq *ss, seq_t pivot);
void free_tree(tree_t *t);
seq_t selMinH(size_t h, vec_seq *ss) {
    if (h == 0) {
        return CONS_ST(0, 1, 2, 3);
    }
    if (vec_size(ss) <= 2) { // from 57s to 42s
        return vec_seq_idx(ss, 0);
    }
    vec_seq *eq = eqk(ss, ss);
    size_t mh = -1;
    size_t i, mi = -1;
    for (i = 0; i != vec_size(eq); i++) {
        tree_t *t = buildTh1(selMinH, h, ss, vec_seq_idx(eq, i));
        size_t h0 = height(t, 0);
        free_tree(t);
        if (h0 < mh) {
            mh = h0, mi = i;
            if (mh == vec_size(ss) - 1) { // from 71s to 57s
                break;
            }
        }
    }
    assert(mi != -1);
    seq_t r = vec_seq_idx(eq, mi);
    free_eqkg(eq);
    return r;
}

typedef seq_t (selectCandFn)(size_t h, vec_seq *ss, vec_seq *cand);
tree_t *buildThc(selectCandFn f, size_t h, vec_seq *ss, vec_seq *cand);
tree_t *buildThc1(selectCandFn f, size_t h, vec_seq *ss, seq_t pivot);

#define INVALID_SEQ CONS_ST(0,0,0,0)
#define INVALID_CODE (-1)

// #define CACHE_LEN 1021
// #define CACHE_LEN 10223
#define CACHE_LEN 65521 // best one
// #define CACHE_LEN 130171

typedef struct {
    size_t code;
    vec_seq *key;
    seq_t val;
} slot_t;

slot_t cache_array[CACHE_LEN];
size_t active = 0, hit = 0, miss = 0;

void cache_init() {
    memset(cache_array, 0x00, sizeof(cache_array));
}

void cache_deinit() {
    size_t i;
    for (i = 0; i != CACHE_LEN; i++) {
        slot_t *p = cache_array + i;
        if (p->key != NULL) {
            vec_deinit(p->key);
        }
    }
}

/** map ss to its best pivot
 * hash vec_seq with length & its elements
 * only store the big seq? or little seq?
 * Limited cache, update???
 */
size_t hash(vec_seq *ss) {
    size_t i, s = 0;
    for (i = 0; i != vec_size(ss); i++) {
        s += vec_seq_idx(ss, i);
    }
    return s;
}

bool vec_seq_eq(vec_seq *a, vec_seq *b) {
    if (a == NULL || b == NULL) {
        return false;
    }
    if (vec_size(a) != vec_size(b)) {
        return false;
    }
    size_t i;
    for (i = 0; i != vec_size(a); i++) {
        if (vec_seq_idx(a, i) != vec_seq_idx(b, i)) {
            return false;
        }
    }
    return true;
}

seq_t cache_fetch(vec_seq *ss) {
    size_t code = hash(ss);
    slot_t *p = cache_array + (code % CACHE_LEN);
    if (p->code == code) {
        if(vec_seq_eq(ss, p->key)) {
            hit++;
            return p->val;
        }
    }
    miss++;
    return INVALID_SEQ;
}

void cache_put(vec_seq *ss, seq_t pivot) {
    size_t code = hash(ss);
    slot_t *p = cache_array + (code % CACHE_LEN);
    if (p->key != NULL) {
        vec_deinit(p->key);
    } else {
        active++;
    }
    p->code = code, p->key = vec_seq_dup(ss), p->val = pivot;
}

#define CACHE 1
/** TODO:
 * it may duplicate same SS and CAND, so we could memoize result
 * Why it duplicate?
 * different pivot, may give same partition result
 */
seq_t selMinHC(size_t h, vec_seq *ss, vec_seq *cand) {
    if (h == 0) {
        return CONS_ST(0, 1, 2, 3);
    }
    if (vec_size(ss) <= 2) {
        return vec_seq_idx(ss, 0);
    }
    if (vec_size(cand) == 1) {
        return vec_seq_idx(cand, 0);
    }
#if CACHE
    seq_t c = cache_fetch(ss);
    if (c != INVALID_SEQ) {
        // printf("ss: "); show_seq_vec(ss);
        // printf("cached pivot: "); show_seq(c);
        return c;
    }
#endif
    vec_seq *e = NULL;
    if (h >= 3) {
        e = eqk(ss, ss);
    } else {
        e = eqk(ss, cand);
    }
    size_t mh = -1, i, mi = -1;
    for (i = 0; i != vec_size(e); i++) {
        tree_t *t = buildThc1(selMinHC, h, ss, vec_seq_idx(e, i));
        size_t h0 = height(t, 0);
        free_tree(t);
        if (h0 < mh) {
            mh = h0, mi = i;
            if (mh == vec_size(ss) - 1) {
                break; // least height, do not need more try
            }
        }
    }
    assert(mi != -1);
    seq_t r = vec_seq_idx(e, mi);
#if CACHE
    cache_put(ss, r);
#endif
    if (vec_size(ss) > 100) {
    // if (1) {
        // printf("ss: "); show_seq_vec(ss);
        // printf("cand: "); show_seq_vec(cand);
        // printf("eq: "); show_seq_vec(e);
        // printf("pivot: "); show_seq(r);
        printf("h=%zu n=%zu cand=%zu eq=%zu mh=%zu self=%s\n",
               h, vec_size(ss), vec_size(cand), vec_size(e), mh, show_bool(vec_seq_elem(ss, r)));
    }
    free_eqkg(e);
    // if (vec_size(ss) == 360) {
    //     printf("active=%zu hit=%zu miss=%zu\n", active, hit, miss);
    //     exit(-1);
    // }
    return r;
}

tree_t *buildThf(selectFn f, size_t h, vec_seq *ss);

tree_t *buildTh1(selectFn f, size_t h, vec_seq *ss, seq_t pivot) {
    tree_t *t = malloc(sizeof(tree_t));
    if (vec_size(ss) == 1) {
        // TODO: put chk0 position or not, if not, we need guess one more time, even only one
        // candicate
        memset(t, 0x00, sizeof(tree_t));
        t->nd = LEAF;
        t->x = vec_seq_idx(ss, 0);
        return t;
    }
    t->nd = NODE;
    t->x = pivot;
    vec_seq **va = sieve(ss, pivot);
    size_t i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        t->child[i] = buildThf(f, h + 1, va[i]);
    }
    free_sieve(va);
    return t;
}

tree_t *buildThf(selectFn f, size_t h, vec_seq *ss) {
    if (vec_size(ss) == 0) {
        return NULL;
    }
    seq_t pivot = f(h, ss);
    tree_t *t = buildTh1(f, h, ss, pivot);
    return t;
}

tree_t *buildThc1(selectCandFn f, size_t h, vec_seq *ss, seq_t pivot) {
    tree_t *t = malloc(sizeof(tree_t));
    if (vec_size(ss) == 1) {
        memset(t, 0x00, sizeof(tree_t));
        t->nd = LEAF, t->x = vec_seq_idx(ss, 0);
        return t;
    }
    t->nd = NODE, t->x = pivot;
    vec_seq **va = sieve(ss, pivot);
    vec_seq *e = eqk2(va);
    size_t i;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        t->child[i] = buildThc(f, h + 1, va[i], e);
    }
    free_sieve(va);
    vec_deinit(e);
    return t;
}

tree_t *buildThc(selectCandFn f, size_t h, vec_seq *ss, vec_seq *cand) {
    if (vec_size(ss) == 0) {
        return NULL;
    }
    seq_t pivot = f(h, ss, cand);
    tree_t *t = buildThc1(f, h, ss, pivot);
    return t;
}

void free_tree(tree_t *t) {
    if (t == NULL) {
        return;
    }
    if (t->nd == LEAF) {
        free(t);
    } else {
        size_t i;
        for (i = 0; i != CHK_KIND_NUM; i++) {
            free_tree(t->child[i]);
        }
        free(t);
    }
}

tree_t *buildT(vec_seq *ss) {
    cache_init();
    // return buildThf(first, 0, ss); // 28024, 0.01s
    // return buildThf(selMaxE, 0, ss); // 26780, 0.55s
    // return buildThf(selMinH, 0, ss); // 26688, 92.13s -> 42s
    vec_seq *c0 = vec_seq_init();
    vec_seq_add(c0, CONS_ST(0, 1, 2, 3));
    return buildThc(selMinHC, 0, ss, c0);
    // h >= 1, 26688, 22s
    // h >= 2, 26478, 218.29s
    // h >= 3, TODO
}

size_t cnt(tree_t *t) {
    if (t == NULL) {
        return 0;
    }
    size_t sub = 0;
    if (t->nd == NODE) {
        size_t i;
        for (i = 0; i != CHK_KIND_NUM - 1; i++) {
            sub += cnt(t->child[i]);
        }
    }
    return sub + 1;
}

size_t height(tree_t *t, size_t h) {
    if (t == NULL) {
        return 0;
    }
    size_t sub = 0;
    if (t->nd == NODE) {
        size_t i;
        for (i = 0; i != CHK_KIND_NUM - 1; i++) {
            sub += height(t->child[i], h + 1);
        }
    }
    return sub + h;
}

void showT(tree_t *t, size_t h) {
    if (t == NULL) {
        return;
    }
    printf("%*s", (int)h, ""); show_seq(t->x);
    if (t->nd == NODE) {
        size_t i;
        // skip to duplicate show guess(4,4) result
        for (i = 0; i != CHK_KIND_NUM - 1; i++) {
            showT(t->child[i], h + 1);
        }
    }
}

static inline uint32_t cons_st(uint32_t *st) {
    return st[0] + (st[1] << 8) + (st[2] << 16) + (st[3] << 24);
}

static inline seq_t inc_seq(seq_t n) {
    char *x = (char *)&n;
    char carriage = 0;
    size_t i;
    for (i = K - 1, x[i] += 1; i >= 0; i--) {
        x[i] += carriage;
        carriage = x[i] / 10;
        if (carriage == 0) {
            break;
        }
        x[i] -= carriage * 10;
    }
    return n;
}

vec_seq *init_lst() {
    vec_seq *lst = vec_seq_init();
    unsigned int st0[K] = {0, 1, 2, 3};
    unsigned int st1[K] = {9, 8, 7, 7};     // next of 9876
    seq_t i, b = cons_st(st0), e = cons_st(st1);
    for (i = b; i != e; i = inc_seq(i)) {
        if (is_perm(i)) {
            vec_seq_add(lst, i);
        }
    }
    return lst;
}

#if 0
void init_comb_lst() {
    unsigned int st[K] = {0, 1, 2, 3};
    int i;
    for (i = 0; i < 210; i++) {
        lst[i] = cons_st(st);
        int j;
        for (j = K - 1; j >= 0; j--) {
            st[j]++;
            if (st[j] != N + (j - (K - 1))) {
                break;
            }
        }
        for (j++; j < K; j++) {
            st[j] = st[j - 1] + 1;
        }
    }
}

void init_perm_lst() {
    unsigned int st[K] = {0, 1, 2, 3};
    int i;
    for (i = 0; i != 24; i++) {
        lst[i] = cons_st(st);
        int j;
        // for ()

    }

}

void show_lst(seq_t *s, size_t n) {
    size_t i;
    for (i = 0; i != n; i++) {
        show_seq(s[i]);
        // char *x = (char *)&s[i];
        // printf("%d%d%d%d\n", x[0],x[1],x[2],x[3]);
    }
}

#endif
int test_sieveg() {
    vec_seq *lst = init_lst();
    size_t *r = sieveg(lst, CONS_ST(0, 1, 2, 3));
    show_chk(r);
    return 0;
}

int test_sieve() {
    vec_seq *lst = init_lst();
    vec_seq **va = sieve(lst, CONS_ST(0, 1, 2, 3));
    size_t i, j;
    for (i = 0; i != CHK_KIND_NUM; i++) {
        show_seq_vec(va[i]);
    }
    return 0;
}

int test_eqk() {
    vec_seq *lst = init_lst();
    vec_seq **va = sieve(lst, CONS_ST(0, 1, 2, 3));
    vec_seq *e = vec_seq_init();
    size_t i, j, k;
    for (i = 0; i  != CHK_KIND_NUM; i++) {
        // printf("i=%zu :", i); show_seq_vec(va[i]);
        vec_seq *e0 = eqk(va[i], va[i]);
        vec_seq_append(e, e0);
        vec_deinit(e0);
    }
    printf("eqk: "); show_seq_vec(e);
    for (i = 0; i != CHK_KIND_NUM - 1; i++) {
        show_seq_vec(va[i]);
        for (j = 0; j != vec_size(e); j++) {
            // vec_seq **nva = sieve(va[i], vec_seq_idx(e, j));
            // for (k = 0; k != CHK_KIND_NUM; k++) {
            //     printf("k=%zu ", k); show_seq_vec(nva[k]);
            // }
            show_seq(vec_seq_idx(e, j));
            size_t *r = sieveg(va[i], vec_seq_idx(e, j));
            show_chk(r);
            printf("%f\n", entropy(r));
        }
    }
    return 0;
}

int test_build() {
    vec_seq *lst = init_lst();
#if OPT == 2
    chk_init(lst);
#endif
    tree_t *t = buildT(lst);
    // showT(t, 0);
    size_t h = height(t, 1);
    printf("height=%zu\n", h);
    printf("active=%zu hit=%zu miss=%zu\n", active, hit, miss);
    vec_deinit(lst);
    free_tree(t);
    return 0;
}

int main() {
    // test_eqk();
    test_build();
    return 0;
}
