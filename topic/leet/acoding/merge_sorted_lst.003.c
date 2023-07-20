#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int x;
    struct Node *next;
} Node;

int less(Node *a, Node *b) {
    return a->x < b->x;
}

Node *merge(Node *a, Node *b) {
    Node *head, *prev;
    for (head = prev = NULL; a != NULL && b != NULL; ) {
        if (less(a, b)) {
            if (prev != NULL) {
                prev->next = a;
            }
            prev = a;
            a = a->next;
        } else {
            if (prev != NULL) {
                prev->next = b;
            }
            prev = b;
            b = b->next;
        }
        if (head == NULL) {
            head = prev;
        }
    }
    prev->next = (a == NULL) ? b : a;
    return head;
}

Node *build(int *a, size_t n) {
    Node *head = NULL, *prev = NULL;
    size_t i;
    for (i = 0; i != n; i++) {
        Node *n = malloc(sizeof(Node));
        n->x = a[i];
        n->next = NULL;

        if (prev != NULL) {
            prev->next = n;
        }
        prev = n;
        if (head == NULL) {
            head = prev;
        }
    }
    return head;
}

void show(Node *p) {
    for (; p != NULL; p = p->next) {
        printf("%d, ", p->x);
    }
    printf("\n");
}

void dtor(Node *p) {
    Node *next;
    for (; p != NULL; p = next) {
        next = p->next;
        free(p);
    }
}

int main() {
    int a[] = {4, 8, 15, 19};
    int b[] = {7, 9, 10, 16, 17};
    Node *la = build(a, sizeof(a) / sizeof(a[0]));
    Node *lb = build(b, sizeof(b) / sizeof(b[0]));
    show(la);
    show(lb);
    Node *n = merge(la, lb);
    show(n);

    dtor(n);
    return 0;
}

