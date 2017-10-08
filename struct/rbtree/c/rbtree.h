#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stdbool.h>

typedef void *RBTree;

typedef int keyType;
typedef void *valType;


RBTree  rbtree_create();
void    rbtree_put(RBTree p, keyType key, valType val);
valType rbtree_get(RBTree p, keyType key);
bool    rbtree_contains(RBTree p, keyType key);
void    rbtree_show(RBTree p);

#endif
