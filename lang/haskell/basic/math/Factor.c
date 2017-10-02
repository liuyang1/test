#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    bool *a;
    unsigned int size;
    unsigned int idx;
} PrimeGen;

void pg_destroy(PrimeGen *p) {
    if (p->a) {
        free(p->a);
    }
    free(p);
}

PrimeGen *pg_create(unsigned int size) {
    PrimeGen *p = malloc(sizeof(PrimeGen));
    if (p == NULL) {
        return p;
    }
    p->a = malloc(sizeof(bool) * size);
    if (p->a == NULL) {
        goto fail;
    }
    unsigned int i;
    for (i = 0; i != size; i++) {
        p->a[i] = true;
    }
    p->a[0] = p->a[1] = false;
    p->size = size;
    p->idx = 2;

    return p;
fail:
    pg_destroy(p);
    return NULL;
}

unsigned int pg_next(PrimeGen *p) {
    unsigned int ret = 0, j;
    for (ret = 0; p->idx < p->size; p->idx++) {
        if (p->a[p->idx]) {
            ret = p->idx;
            break;
        }
    }
    for (j = p->idx * 2; j < p->size; j += p->idx) {
        p->a[j] = false;
    }
    p->idx++;

    return ret;
}

int test_pg(unsigned int num) {
    PrimeGen *p = pg_create(num);

    unsigned int ret = 0;
    printf("[");
    const char *prefix = "";
    while ((ret = pg_next(p)) != 0) {
        printf("%s%d", prefix, ret);
        prefix = ", ";
    }
    printf("]\n");

    pg_destroy(p);
    return 0;
}

typedef struct {
    unsigned int v;
    PrimeGen *pg;
    unsigned int cp; // current prime
} FactorGen;

void fg_destroy(FactorGen *p) {
    if (p->pg) {
        pg_destroy(p->pg);
    }
    free(p);
}

FactorGen *fg_create(unsigned int v) {
    FactorGen *p = malloc(sizeof(FactorGen));
    if (p == NULL) {
        return NULL;
    }
    p->pg = pg_create(v + 1);
    if (p->pg == NULL) {
        goto fail;
    }
    p->cp = pg_next(p->pg);
    p->v = v;
    return p;
fail:
    fg_destroy(p);
    return NULL;
}

unsigned int fg_next(FactorGen *p) {
    do {
        if (p->v == 1) {
            return 0;
        }
        if (p->v % p->cp == 0) {
            p->v /= p->cp;
            return p->cp;
        }
    } while (p->cp = pg_next(p->pg));
}

int test_fg(unsigned int v) {
    FactorGen *p = fg_create(v);

    printf("factor(%d) = [", v);
    const char *prefix = "";
    unsigned int ret = 0;
    while ((ret = fg_next(p)) != 0) {
        printf("%s%d", prefix, ret);
        prefix = ", ";
    }
    printf("]\n");

    fg_destroy(p);
    return 0;
}

int test_group_fg() {
    test_fg(120);
    test_fg(1000123);
    test_fg(71023);
    test_fg(100123878);
    return 0;
}

typedef struct {
    unsigned int last;
    FactorGen *fg;
} FactorGroupGen;

typedef struct {
    unsigned int factor;
    unsigned int freq;
} FactorFreq;

void fgg_destroy(FactorGroupGen *p) {
    if (p->fg) {
        fg_destroy(p->fg);
    }
    free(p);
}

FactorGroupGen *fgg_create(unsigned int v) {
    FactorGroupGen *p = malloc(sizeof(FactorGroupGen));
    if (p == NULL) {
        return p;
    }
    p->fg = fg_create(v);
    p->last = 0;

    p->last = fg_next(p->fg);
    return p;
fail:
    fgg_destroy(p);
    return NULL;
}

FactorFreq fgg_next(FactorGroupGen *p) {
    FactorFreq r;
    r.factor = p->last;
    r.freq = 1;
    if (p->last == 0) {
        r.freq = 0;
        return r;
    }
    while ((p->last = fg_next(p->fg)) == r.factor) {
        r.freq++;
    }
    return r;
}

int test_fgg(unsigned int v) {
    FactorGroupGen *p = fgg_create(v);

    printf("factorgroup(%d) = [", v);
    const char *prefix = "";
    FactorFreq r;
    do {
        r = fgg_next(p);
        if (r.freq == 0) {
            break;
        }
        printf("%s(%d, %d)", prefix, r.factor, r.freq);
        prefix = ", ";
    } while (true);
    printf("]\n");

    fgg_destroy(p);
    return 0;
}

int test_group_fgg() {
    test_fgg(120);
    test_fgg(100123);
    test_fgg(71023);
    test_fgg(100123878);
    return 0;
}

int main() {
    test_pg(16 * 1024);
    test_group_fg();
    test_group_fgg();
    return 0;
}
