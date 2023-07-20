#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int x;
    struct Node *left, *right;
} Node;

Node *build_iter(int a[], size_t cap, size_t i) {
    if (i >= cap) {
        return NULL;
    }
    Node *n = malloc(sizeof(Node));
    n->x = a[i];
    n->left = build_iter(a, cap, 2 * i);
    n->right = build_iter(a, cap, 2 * i + 1);
    return n;
}

Node *build(int a[], size_t cap) {
    if (cap == 1) {
        return NULL;
    }
    return build_iter(a, cap, 1);
}

void show_iter(Node *n) {
    if (n == NULL) {
        printf("nil");
        return;
    }
    printf("%d (", n->x);
    show_iter(n->left);
    printf(", ");
    show_iter(n->right);
    printf(")");
}

void show(Node *n) {
    show_iter(n);
    printf("\n");
}

void dtor(Node *n) {
    if (n == NULL) {
        return;
    }
    dtor(n->left);
    dtor(n->right);
    free(n);
}

bool is_bst_iter(Node *n, int lower_valid, int lower, int upper_valid, int upper) {
    if (n == NULL) {
        return true;
    }
    if (lower_valid && n->x < lower) {
        return false;
    }
    if (upper_valid && n->x > upper) {
        return false;
    }
    return is_bst_iter(n->left, lower_valid, lower, 1, n->x) &&
        is_bst_iter(n->right, 1, n->x, upper_valid, upper);
}

bool is_bst(Node *n) {
    return is_bst_iter(n, 0, 0, 0, 0);
}

int main() {
    int a[] = {0, 100, 50, 200, 25, 75, 90, 300};
    Node *n = build(a, sizeof(a) / sizeof(a[0]));
    show(n);
    bool r = is_bst(n);
    printf("%d\n", r);
    dtor(n);
    return 0;
}
