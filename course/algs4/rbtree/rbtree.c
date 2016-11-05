#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef int keyType;
typedef void *valType;

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

void node_init(Node *n, keyType key, valType val, int num, Color color) {
    n->key = key;
    n->val = val;
    n->num = num;
    n->color = color;
    n->left = n->right = NULL;
}

int node_size(Node *n) {
    return n == NULL ? 0 : n->num;
}

bool node_isRed(Node *n) {
    return n == NULL ? false : n->color == RED;
}

Node *node_rotateLeft(Node *h) {
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = RED;
    x->num = h->num;
    h->num = 1 + node_size(h->left) + node_size(h->right);
    return x;
}

Node *node_rotateRight(Node *h) {
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = h->color;
    h->color = RED;
    x->num = h->num;
    h->num = 1 + node_size(h->left) + node_size(h->right);
    return x;
}

void node_flipColors(Node *h) {
    h->color = RED;
    h->left->color = BLACK;
    h->right->color = BLACK;
}

int node_height(Node *h) {
    if (h == NULL) {
        return 0;
    }
    int left = node_height(h->left);
    int right = node_height(h->right);
    return MAX(left, right) + 1;
}

#define LOG
Node *node_put(Node *h, keyType key, valType val) {
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

valType node_get(Node *h, keyType key) {
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

bool node_cotains(Node *h, keyType key) {
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

void node_show(Node *h, int level) {
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

typedef struct {
    Node *root;
} RBTree;

RBTree *rbtree_create() {
    RBTree *r = malloc(sizeof(RBTree));
    r->root = NULL;
    return r;
}

void rbtree_put(RBTree *t, keyType key, valType val) {
    t->root = node_put(t->root, key, val);
    t->root->color = BLACK; // always reset root to BLACK color
}

valType rbtree_get(RBTree *t, keyType key) {
    return node_get(t->root, key);
}

bool rbtree_contains(RBTree *t, keyType key) {
    return node_cotains(t->root, key);
}

void rbtree_show(RBTree *t) {
    node_show(t->root, 0);
}

int rbtree_height(RBTree *t) {
    return node_height(t->root);
}

int rbtree_size(RBTree *t) {
    return node_size(t->root);
}

int rbtree_isEmpty(RBTree *t) {
    return rbtree_size(t) == 0;
}

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
