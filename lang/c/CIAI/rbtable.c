/*
 * Table with red-black tree
 * cmp function must return order
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mem.h"
#include "assert.h"
#include "table.h"

#define T RbTable_T

typedef enum {
    BLACK = false,
    RED = true,
} Color;

typedef struct Node {
    Key_T key;
    Val_T val;
    int num;
    Color color;
    struct Node *left;
    struct Node *right;
} Node;

struct T {
    Node *root;
    CmpFn cmpFn;
    // HashFn hashFn;
    unsigned int timestamp;
};

static void node_init(Node *n, Key_T key, Val_T val, int num, Color color) {
    n->key = key;
    n->val = val;
    n->num = num;
    n->color = color;
    n->left = n->right = NULL;
}

static int node_length(Node *n) {
    return n == NULL ? 0 : n->num;
}

static int node_isRed(Node *n) {
    return n == NULL ? false : n->color == RED;
}

static Node *node_rotateLeft(Node *h) {
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = RED;
    x->num = h->num;
    h->num = 1 + node_length(h->left) + node_length(h->right);
    return x;
}

static Node *node_rotateRight(Node *h) {
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = h->color;
    h->color = RED;
    x->num = h->num;
    h->num = 1 + node_length(h->left) + node_length(h->right);
    return x;
}

static void node_flipColors(Node *h) {
    h->color = RED;
    h->left->color = h->right->color = BLACK;
}

/* not used
#define MAX(a, b) ((a) > (b) ? (a) : (b))
static int node_height(Node *h) {
    if (h == NULL) {
        return 0;
    }
    int left = node_height(h->left);
    int right = node_height(h->right);
    return 1 + MAX(left, right);
}
*/

static Node *node_put(Node *h, Key_T key, Val_T val, CmpFn cmpFn, Val_T *old) {
    if (h == NULL) {
        h = malloc(sizeof(Node));
        node_init(h, key, val, 1, RED);
        return h;
    }
    int c = cmpFn(key, h->key);
    if (c < 0) {
        h->left = node_put(h->left, key, val, cmpFn, old);
    } else if (c > 0) {
        h->right = node_put(h->right, key, val, cmpFn, old);
    } else {
        *old = h->val;
        h->val = val;
    }

    if (node_isRed(h->right) && !node_isRed(h->left)) {
        h = node_rotateLeft(h);
    }
    if (node_isRed(h->left) && node_isRed(h->left->left)) {
        h = node_rotateRight(h);
    }
    if (node_isRed(h->left) && node_isRed(h->right)) {
        node_flipColors(h);
    }
    h->num = 1 + node_length(h->left) + node_length(h->right);
    return h;
}

static Val_T node_get(Node *h, Key_T key, CmpFn cmpFn) {
    if (h == NULL) {
        return NULL;
    }
    int c = cmpFn(key, h->key);
    if (c < 0) {
        return node_get(h->left, key, cmpFn);
    } else if (c > 0) {
        return node_get(h->right, key, cmpFn);
    } else {
        return h->val;
    }
}

#if 0
static Val_T node_remove(Node *h, Key_T key, CmpFn cmpFn) {
    if (h == NULL) {
        return NULL;
    }
    int c = cmpFn(key, h->key);
    if (c < 0) {
        return node_remove(h->left, key, cmpFn);
    } else if (c > 0) {
        return node_remove(h->right, key, cmpFn);
    } else {
    
    }
}
#endif


// interface function
T RbTable_new(CmpFn cmpFn) {
    T table;
    table = ALLOC(sizeof(*table));
    table->cmpFn = cmpFn;
    table->timestamp = 0;
    table->root = NULL;
    return table;
}

void RbTable_free(T *table) {
    assert(table && *table);

}

int RbTable_length(T table) {
    return node_length(table->root);
}

Val_T RbTable_put(T table, Key_T key, Val_T val) {
    Val_T old;
    table->root = node_put(table->root, key, val, table->cmpFn, &old);
    return old;
}

Val_T RbTree_get(T table, Key_T key) {
    return node_get(table->root, key, table->cmpFn);
}

#if 0
Val_T RbTree_remove(T table, Key_T key) {
    if (h == NULL) {
        return NULL;
    }
    int c = cmpFn(key, h->key);
    if (c < 0) {
        return node_remove(h->left, )
    }

}
#endif
