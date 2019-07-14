/**
 * CLRS 11.1-2 Exercise
 *
 * Bit Vector / Bit Map
 *
 * - Use uint32_t as basic element, as our target is 32bit machine
 *   Of course, may change to uint64_t if target is 64bit machine
 * - Simple module, header file module design
 *
 */
#ifndef _BITVECTOR_H_
#define _BITVECTOR_H_

#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BITS        32

typedef struct {
    size_t len;
    size_t cap;
    uint32_t a[0];
} BVec;

static inline int bvec_get(BVec *p, uint32_t i, bool *b) {
    assert(b != NULL);
    assert(p != NULL);
    if (i >= p->len) {
        return -ERANGE;
    }
    *b = (p->a[i / BITS] & (1 << (i % BITS))) != 0;
    return 0;
}

static inline void bvec_show(BVec *p) {
    assert(p != NULL);
    printf("bitvector p=%p len=%u cap=%u\n", p, p->len, p->cap);
    uint32_t i;
    /** show whole table */
    const uint32_t chunk = 10;
    for (i = 0; i != p->len; i++) {
        if (i % chunk == 0) {
            printf("%08d: ", i);
        }
        if (i % (chunk / 2) == 0) {
            printf(" ");
        }
        bool v;
        bvec_get(p, i, &v);
        printf("%d ", v);
        if ((i + 1) % chunk == 0) {
            printf("\n");
        }
    }
    if (i % chunk != 0) {
        printf("\n");
    }
    /** show the selected index */
    uint32_t j, cnt = 0;
    for (i = 0; i != p->cap; i++) {
        uint32_t v = p->a[i];
        // stupid way: check bit one by one
        for (j = 0; j != BITS; j++) {
            if (v & 1) {
                cnt++;
                printf("%d, ", i * BITS + j);
                if (cnt + 1 % chunk == 0) {
                    printf("\n");
                }
            }
            v = v >> 1;
        }
    }
    if (cnt % chunk != 0) {
        printf("\n");
    }
}

static inline int bvec_reset(BVec *p) {
    assert(p != NULL);
    memset(p->a, 0x00, p->cap * sizeof(uint32_t));
}

static inline int bvec_create(size_t len, void **pbv) {
    assert(pbv != NULL);
    *pbv = NULL;
    // align
    size_t sz = (len + BITS - 1) / BITS;
    BVec *p = (BVec *)malloc(sizeof(BVec) + sizeof(uint32_t) * sz);
    if (p == NULL) {
        return -ENOMEM;
    }
    p->len = len;
    p->cap = sz;
    bvec_reset(p);
    // bvec_show(p);
    *pbv = p;
    return 0;
}

static inline int bvec_destroy(BVec *p) {
    assert(p != NULL);
    free(p);
    return 0;
}

static inline int bvec_set(BVec *p, uint32_t i) {
    assert(p != NULL);
    if (i >= p->len) {
        return -ERANGE;
    }
    uint32_t j = i / BITS, k = i % BITS;
    p->a[j] = p->a[j] | (1 << k);
    return 0;
}

static inline int bvec_unset(BVec *p, uint32_t i) {
    assert(p != NULL);
    if (i >= p->len) {
        return -ERANGE;
    }
    uint32_t j = i / BITS, k = i % BITS;
    p->a[j] = p->a[j] & (!(1 << k));
    return 0;
}

#undef  BITS

#endif
