#include "bitvector.h"
#include <stdio.h>

int main() {
    int ret = 0;
    uint32_t len = 100;
    void *p;
    ret = bvec_create(len, &p);
    bvec_set(p, 30);
    bvec_show(p);
    bvec_unset(p, 30);
    bvec_show(p);

    bvec_reset(p);

    uint32_t i;
    for (i = 0; i != 13; i++) {
        uint32_t idx = rand() % len;
        bvec_set(p, idx);
        printf("set idx=%d\n", idx);
    }
    bvec_show(p);

    bvec_destroy(p);
    return 0;

}
