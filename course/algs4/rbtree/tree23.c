#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef int Key_T;
typedef void *Val_T;
#define INVALIDKEY (-1)
#define INVALIDVAL NULL

int keycmp(Key_T a, Key_T b) {
    return a - b;
}

// support 2-3-4 tree
// 4 sub-node to transform to 2-3 tree
#define CHILDNUM 4
#define VALNUM  (CHILDNUM - 1)
typedef struct Node {
    int num;
    struct Node *childs[CHILDNUM];
    Key_T keys[VALNUM];
    Val_T vals[VALNUM];
} Node;

static void node_reset(Node *h) {
    h->num = 0;
    int i;
    for (i = 0; i != CHILDNUM; i++) {
        h->childs[i] = NULL;
    }
    for (i = 0; i != VALNUM; i++) {
        h->keys[i] = INVALIDKEY;
        h->vals[i] = INVALIDVAL;
    }
}
static void node_init4(Node *h,
                       Key_T key0, Val_T val0,
                       Key_T key1, Val_T val1,
                       Key_T key2, Val_T val2) {
    node_reset(h);
    h->num = 3;
    h->keys[0] = key0;
    h->keys[1] = key1;
    h->keys[1] = key2;
    h->vals[0] = val0;
    h->vals[1] = val1;
    h->vals[1] = val2;
}
static void node_init3(Node *h,
                       Key_T key0, Val_T val0,
                       Key_T key1, Val_T val1) {
    node_reset(h);
    h->num = 3;
    h->keys[0] = key0;
    h->keys[1] = key1;
    h->vals[0] = val0;
    h->vals[1] = val1;
}
static void node_init2(Node *h, Key_T key, Val_T val) {
    node_reset(h);
    h->num = 2;
    h->keys[0] = key;
    h->vals[0] = val;
}

static Node *node_split(Node *h, Node *parent) {
    if (h->num != 4) {
        return h;
    }
    Node *node = malloc(sizeof(Node));
    node_init2(node, h->keys[2], h->vals[2]);
    node->childs[0] = h->childs[2];
    node->childs[1] = h->childs[3];

    node_init2(h, h->keys[0], h->vals[0]);
    h->childs[2] = h->childs[3] = NULL;

    parent->keys[2] = parent->keys[1];
    parent->vals[2] = parent->vals[1];
    parent->childs[3] = parent->childs[2];
    parent->childs[1] = h;
    parent->childs[2] = node;

    return h;
}
static bool node_isLeaf(Node *h) {
    int i;
    for (i = 0; i != CHILDNUM; i++) {
        if (h->childs[i] != 0) {
            return false;
        }
    }
    return true;
}
static Val_T node_get(Node *h, Key_T key) {
    if (h == NULL) {
        return NULL;
    }
    int i;
    for (i = 0; i != h->num - 1; i++) {
        int cmp = keycmp(key, h->keys[i]);
        if (cmp < 0) {
            return node_get(h->childs[i], key);
        } else if (cmp == 0) {
            return h->vals[i];
        } else {
            continue;
        }
    }
    return node_get(h->childs[i], key);
}

static Node *node_put(Node *h, Key_T key, Val_T val) {
    if (h == NULL) {
        h = malloc(sizeof(Node));
        node_init2(h, key, val);
        return h;
    }
    int i;
    if (node_isLeaf(h)) {
        for (i = 0; i != h->num - 1; i++) {
            int cmp = keycmp(key, h->keys[i]);
            if (cmp < 0) {
                node_doPut(h, i, key, val);
            }
        
        }
    }
    for (i = 0; i != h->num - 1; i++) {
        int cmp = keycmp(key, h->keys[i]);
        if (cmp < 0) {
            h->childs[i] = node_put(h->childs[i], key, val);
            return h;
        } else if (cmp == 0) {
            printf("warning: not support same key=%d\n", key);
            return h;
        }
    }
    h->childs[i] = node_put(h->childs[i], key, val);
    return h;
}

typedef struct {
    Node *root;
} Tree23;

void tree23_get(Tree23 *t, Key_T key) {

}
