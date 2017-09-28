#include "leet.h"

// Counter struct begin
typedef struct HashNode {
    struct HashNode *next;
    int val;
    int freq;
} HashNode;

void node_show(void *p) {
    HashNode *node = p;
    if (p == NULL) {
        printf("%p", p);
    } else {
        printf("%d(%d)", node->val, node->freq);
    }
}

typedef struct HashTbl {
    int cap;
    HashNode **tbl;
} HashTbl;

HashTbl *ht_create(int cap);
void ht_destory(HashTbl *t);
void ht_put(HashTbl *t, int x);
HashNode *ht_next(HashTbl *t, HashNode *p);
void ht_show(HashTbl *t);

HashTbl *ht_create(int cap) {
    if (cap == 0) {
        cap = 2048;
    }
    HashNode **tbl = malloc(sizeof(HashNode *) * cap);
    memset(tbl, 0, sizeof(HashNode *) * cap);

    HashTbl *r = malloc(sizeof(HashTbl));
    r->cap = cap;
    r->tbl = tbl;
    return r;
}

void ht_destory(HashTbl *t) {
    int i;
    for (i = 0; i != t->cap; i++) {
        HashNode *p = t->tbl[i];
        for (p = t->tbl[i]; p != NULL;) {
            HashNode *n = p->next;
            free(p);
            p = n;
        }
    }
    free(t->tbl);
    free(t);
}

bool needAdjust(int a, int b) {
    return a < b * (a / b) &&
           (a < 0) != (b < 0) &&
           a % b != 0;
}

int mod(int a, int b) {
    if (needAdjust(a, b)) {
        return a % b + b;
    } else {
        return a % b;
    }
}

int ht_hash(HashTbl *t, int x) {
    // correct mod function for hash
    return mod(x, t->cap);
}

void ht_put(HashTbl *t, int x) {
    int i = ht_hash(t, x);
    HashNode *p;
    for (p = t->tbl[i]; p != NULL; p = p->next) {
        if (p->val == x) {
            p->freq++;
            return;
        }
    }

    HashNode *node = malloc(sizeof(HashNode));
    node->val = x;
    node->freq = 1;
    // insert to head of link list
    node->next = t->tbl[i];
    t->tbl[i] = node;
}

/** iterate function on hash table
 * - start iterate on hash table with NULL
 * - continue to find until not NULL
 * -  if have next node, return next
 * -  if have no next node, continue to iterate
   - to end of table, return NULL again
 */
HashNode *ht_next(HashTbl *t, HashNode *p) {
    int i = 0;
    if (p != NULL) {
        if (p->next == NULL) {
            i = ht_hash(t, p->val) + 1;
        } else {
            return p->next;
        }
    }
    for (i != t->cap; i != t->cap; i++) {
        p = t->tbl[i];
        if (p != NULL) {
            return p;
        }
    }
    return NULL;
}

void ht_show(HashTbl *t) {
    printf("HashTbl: %p cap=%d tbl=%p\n", t, t->cap, t->tbl);
    bool first = true;
    int i;
    HashNode *p = NULL;
    const char *fmt;

    printf("[");
    while ((p = ht_next(t, p)) != NULL) { // iterate style
        if (first) {
            first = false;
        } else {
            printf(", ");
        }
        node_show(p);
    }
    printf("]\n");
}

// Counter struct END ---------------------------------------------------------

// Limit Capicity Priority Queue struct ---------------------------------------

typedef struct {
    int cap; // original capcity setting by user
    int size; // size of arr
    int num; // num of valid node number
    void **arr;
    int (*cmp)(void *, void *); // to compare node
    void (*show)(void *); // to show node
} PQ;

PQ *pq_create(int cap, int (*cmp)(void *, void *), void (*show)(void *)) {
    PQ *p = malloc(sizeof(PQ));
    p->num = 0;
    p->cap = cap;
    // why need extra 2 node?
    // - first node of heap is dummy
    // - When node is to CAP, need extra one to insert to heap, then delete top one.
    p->size = p->cap + 2;
    int size = sizeof(void *) * p->size;
    p->arr = malloc(sizeof(void *) * size);
    p->cmp = cmp;
    p->show = show;
    memset(p->arr, 0, sizeof(void *) * size);
    return p;
}

void pq_destory(PQ *p) {
    // Priority Queue don't care node's memory management
    free(p->arr);
    free(p);
}

void pq_show(PQ *p) {
    printf("Prority Queue: %p cap=%d size=%d num=%d cmp=%p arr=%p\n",
           p, p->cap, p->size, p->num, p->cmp, p->arr);
    const char *delim = "";
    int i;
    printf("[");
    for (i = 0; i != p->size; i++) {
        printf("%s%p", delim, p->arr[i]);
        delim = ", ";
        if (p->show) {
            p->show(p->arr[i]);
        }
    }
    printf("]\n");
}

bool pq_less(PQ *p, int i, int j) {
    return p->cmp(p->arr[i], p->arr[j]) < 0;
}

void pq_exch(PQ *p, int i, int j) {
    void *t = p->arr[i];
    p->arr[i] = p->arr[j];
    p->arr[j] = t;
}

void pq_swim(PQ *p, int i) {
    // if not top node but less, then swim from bottom to up
    while (i > 1 && pq_less(p, i / 2, i)) {
        pq_exch(p, i / 2, i);
        i /= 2;
    }
}

void pq_sink(PQ *p, int i) {
    while (2 * i <= p->num) {
        int j = 2 * i;
        // swap to with larger child node
        if (j < p->num && pq_less(p, j, j + 1)) {
            j++;
        }
        if (!pq_less(p, i, j)) {
            break;
        }
        pq_exch(p, i, j);
        i = j;
    }
}

void *pq_delTop(PQ *p) {
    // - delete top node
    // - swap last node to top
    // - sink new top node to bottom
    void *m = p->arr[1];
    if (m == NULL) {
        return NULL;
    }
    pq_exch(p, 1, p->num);
    p->arr[p->num] = NULL;
    p->num--;
    pq_sink(p, 1);
    return m;
}

void pq_insert(PQ *p, void *node) {
    // - insert node to last one
    // - swim last node to up
    // - if Queue is full, then remove top one
    p->arr[p->num + 1] = node;
    pq_swim(p, p->num + 1);
    p->num++;

    // limit capcity
    if (p->num == p->cap + 1) {
        pq_delTop(p);
    }
}

// Limit Capicity Priority Queue struct END -----------------------------------

int cmpNode(void *a, void *b) {
    HashNode *pa = a, *pb = b;
    return -pa->freq + pb->freq;  // we need min PQ, but not max PQ
}

int *topKFrequent(int *nums, int numsSize, int k, int *returnSize) {
    if (nums == NULL || numsSize == 0) {
        *returnSize = 0;
        return NULL;
    }
    // - hashtable to count frequence of number. O(n)
    // - Priority queue to sort most frequent k element
    // - reverse pop from queue to get result
    HashTbl *t = ht_create(0);
    int i;
    for (i = 0; i != numsSize; i++) {
        ht_put(t, nums[i]);
    }

    PQ *q = pq_create(k, cmpNode, node_show);
    HashNode *p = NULL;
    while ((p = ht_next(t, p)) != NULL) {
        if (p == NULL) {
            return NULL;
        }
        pq_insert(q, p);
    }

    int *ret = malloc(sizeof(int) * k);
    *returnSize = k; // k is always valid
    i = k - 1;
    while ((p = pq_delTop(q)) != NULL) {
        ret[i--] = p->val;
    }

    pq_destory(q);
    // temp node will free within hash table
    ht_destory(t);
    return ret;
}

// test code ------------------------------------------------------------------
int testHashTbl() {
    HashTbl *t = ht_create(10);
    int a[] = {1, 1, 1, 2, 2, 3, 2, 4, 4, 4, 3, 11, 14, 13, 23};
    int i;
    for (i = 0; i != COUNT_OF(a); i++) {
        ht_put(t, a[i]);
    }
    ht_show(t);
    free(t);
    return 0;
}

int cmp(void *a, void *b) {
    return a - b;
}

int testPQ() {
    PQ *p = pq_create(5, cmp, NULL);
    long int i;
    for (i = 10; i != 0; i--) {
        pq_insert(p, (void *)i);
    }
    pq_show(p);
    pq_destory(p);
    return 0;
}

bool unit(int *a, int size, int k, int *e, int esz) {
    int len;
    int *r = topKFrequent(a, size, k, &len);
    bool ret = cmpArr(r, len, e, esz);
    if (!ret) {
        showArr(a, size);
        printf("topKFrequent(arr, %d) == ", k);
        showArr(r, len);
        printf(" ?= ");
        showArr(e, esz);
        printf("a=%p r=%p e=%p size=%d len=%d esz=%d\n",
               a, r, e, size, len, esz);
    }
    printf("%s\n", expect(ret));
    free(r);
    return ret;
}

#define UNIT(a, k, e) unit(a, COUNT_OF(a), k, e, COUNT_OF(e));
int testTopK() {
    int a0[] = {1};
    UNIT(a0, 1, a0);

    int a1[] = {1, 1, 1, 2, 2, 3};
    int e1[] = {1, 2};
    UNIT(a1, 2, e1);

    int a2[] = {};
    int e2[] = {};
    UNIT(a2, 0, e2);

    int a3[] = {1, 2, 3, 34, 5, 6, 7, 8, 123, 23, 423, 623, 423, 111, 1, 1, 1,
                1, 1, 2, 2, 2, 2, 2, 1, 3};
    int e3[] = {1};
    UNIT(a3, 1, e3);

    int a4[] = {-1, -1};
    int e4[] = {-1};
    UNIT(a4, 1, e4);
    return 0;
}

int main() {
    testHashTbl();
    testPQ();
    testTopK();
    return 0;
}
