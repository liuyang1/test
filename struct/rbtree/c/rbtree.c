#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rbtree.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int keycmp(keyType a, keyType b) {
    return a - b;
}

typedef enum {
    BLACK = false,
    RED = true,
} Color;

typedef struct Node {
    keyType key;
    valType val;
    int num;
    Color color;
    struct Node *left;
    struct Node *right;
} Node;

static void node_init(Node *n, keyType key, valType val, int num, Color color) {
    n->key = key;
    n->val = val;
    n->num = num;
    n->color = color;
    n->left = n->right = NULL;
}

static int node_size(Node *n) {
    return n == NULL ? 0 : n->num;
}

static bool node_isRed(Node *n) {
    return n == NULL ? false : n->color == RED;
}

static Node *node_rotateLeft(Node *h) {
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = RED;
    x->num = h->num;
    h->num = 1 + node_size(h->left) + node_size(h->right);
    return x;
}

static Node *node_rotateRight(Node *h) {
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = h->color;
    h->color = RED;
    x->num = h->num;
    h->num = 1 + node_size(h->left) + node_size(h->right);
    return x;
}

static void node_flipColors(Node *h) {
    h->color = RED;
    h->left->color = BLACK;
    h->right->color = BLACK;
}

static int node_height(Node *h) {
    if (h == NULL) {
        return 0;
    }
    int left = node_height(h->left);
    int right = node_height(h->right);
    return MAX(left, right) + 1;
}

// #define LOG printf
#define LOG(x) ((void)(x)) // omit unused statement error

static Node *node_put(Node *h, keyType key, valType val) {
    if (h == NULL) {
        h = malloc(sizeof(Node));
        node_init(h, key, val, 1, RED);
        return h;
    }
    int cmp = keycmp(key, h->key);
    if (cmp < 0) {
        LOG("left\n");
        h->left = node_put(h->left, key, val);
        LOG("left back\n");
    } else if (cmp > 0) {
        LOG("right\n");
        h->right = node_put(h->right, key, val);
        LOG("right back\n");
    } else {
        h->val = val;
    }

    if (node_isRed(h->right) && !node_isRed(h->left)) {
        LOG("rotateLeft\n");
        h = node_rotateLeft(h);
    }
    if (node_isRed(h->left) && node_isRed(h->left->left)) {
        LOG("rotateRight\n");
        h = node_rotateRight(h);
    }
    if (node_isRed(h->left) && node_isRed(h->right)) {
        LOG("flipColors\n");
        node_flipColors(h);
    }
    h->num = 1 + node_size(h->left) + node_size(h->right);
    return h; // return back, this is very important
}

static valType node_get(Node *h, keyType key) {
    if (h == NULL) {
        return NULL;
    }
    int cmp = keycmp(key, h->key);
    if (cmp < 0) {
        return node_get(h->left, key);
    } else if (cmp > 0) {
        return node_get(h->right, key);
    } else {
        return h->val;
    }
}

static bool node_cotains(Node *h, keyType key) {
    if (h == NULL) {
        return false;
    }
    int cmp = keycmp(key, h->key);
    if (cmp < 0) {
        return node_cotains(h->left, key);
    } else if (cmp > 0) {
        return node_cotains(h->right, key);
    } else {
        return true;
    }
}

static void node_show(Node *h, int level) {
    int i;
    for (i = 0; i != level; i++) {
        printf("  ");
    }
    if (h == NULL) {
        printf("nil\n");
    } else {
        printf("key=%d val=%p num=%d color=%s\n",
               h->key, h->val, h->num, h->color ? "RED" : "BLACK");
        node_show(h->left, level + 1);
        node_show(h->right, level + 1);
    }
}

#define T RBTreeIn

typedef struct {
    Node *root;
} T;

static void rbt_in_destroy(T *t) {
    // TODO:
    return;
}
static T *rbt_in_create() {
    T *r = malloc(sizeof(T));
    r->root = NULL;
    return r;
}

static void rbt_in_put(T *t, keyType key, valType val) {
    t->root = node_put(t->root, key, val);
    t->root->color = BLACK; // always reset root to BLACK color
}

static valType rbt_in_get(T *t, keyType key) {
    return node_get(t->root, key);
}

static bool rbt_in_contains(T *t, keyType key) {
    return node_cotains(t->root, key);
}

static void rbt_in_show(T *t) {
    node_show(t->root, 0);
}

static int rbt_in_height(T *t) {
    return node_height(t->root);
}

static int rbt_in_size(T *t) {
    return node_size(t->root);
}

static int rbt_in_isEmpty(T *t) {
    return rbt_in_size(t) == 0;
}

RBTree  rbtree_create() {
    return rbt_in_create();
}

void    rbtree_destroy(RBTree p) {
    rbt_in_destroy(p);
}
void    rbtree_put(RBTree p, keyType key, valType val) {
    rbt_in_put(p, key, val);
}
valType rbtree_get(RBTree p, keyType key) {
    return rbt_in_get(p, key);
}
bool    rbtree_contains(RBTree p, keyType key) {
    return rbt_in_contains(p, key);
}
void    rbtree_show(RBTree p) {
    rbt_in_show(p);
}
