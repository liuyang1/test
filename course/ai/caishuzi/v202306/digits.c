#include <stdio.h>

/** Based on equivalence set */

#define N 10

struct {
    int a[N];
    size_t n;
} Eqset;

struct Eqset* eqset_divide(struct Eqset *e, int take, struct Eqset **res) {
    assert(take >= 1 && take < e->n);
    Eqset *fst = malloc(struct Eqset), *snd = malloc(struct Eqset);
    fst->n = take;
    snd->n = e->n - take;
    memcpy(fst->a, e->a, sizeof(int) * fst->n);
    memcpy(snd->a, e->a + take, sizeof(int) * snd->n);
    *res = snd;
    return fst;
}

struct {
    Eqset *e;
    size_t m;
} EqSets;

/** 手动展开循环的版本 */
  eqsets_gen_comb(struct Eqsets *es) {
    if (es->m == 1) {
        struct Eqset *res;
        struct Eqset *fst = eqest_divide(es->e[0], 4, &res);

    } else if (es->m == 2) {
        struct Eqset *res0, *res1;
        struct Eqset *fst0 = eqset_divide(es->e[0], 3, &res0);
        struct Eqset *fst1 = eqset_divide(es->e[1], 1, &res1)

    }
}

/** 4
 * 分划为1部分，[(4)]
 * 分划为2部分，[(3,1),(2,2),(1,3)]
 * 分划为3部分，[(2,1,1),(1,2,1),(1,1,2)]
 * 分划为4部分，[(1,1,1,1)]
 * 分划为更多部分，[(1,1,1,1,0)],(1,1,1,0,1),...)
 */

/** [1..9]选择4，可选为1234
 * 4A0B，那么1234
 * 2A0B，两个正确，1256，是下一个典型
 * 2A1B，两个正确，1个位置不对，1253是下一个典型
 * 2A2B，两个正确，2个位置不对，1243是下一个典型
 */
