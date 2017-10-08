#include <stdlib.h>

#include "rbtree.h"

int try_rbtree() {
    RBTree *t = rbtree_create();
    long int i;
    for (i = 0; i != 10; i++) {
        rbtree_put(t, random() % 100, (void *)i);
        // rbtree_show(t);
        // printf("--------------------------\n");
    }
    rbtree_show(t);

    return 0;
}

int main() {
    try_rbtree();
    return 0;
}

