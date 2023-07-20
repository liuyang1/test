#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int x;
    struct Node *next;
} Q;

Q *one(int x) {
    Q *q = malloc(sizeof(Q));
    q->x = x;
    q->next = NULL;
    return q;
}

Q* build(int *a, size_t n) {
    Q *head = NULL, *prev = NULL;
    size_t i;
    for (i = 0; i != n; i++) {
        Q *this = one(a[i]);
        if (prev) {
            prev->next = this;
            prev = this;
        } else {
            prev = this;
            head = this;
        }
    }
    return head;
}

void dtor(Q *q) {
    Q *prev = NULL;
    for (; q; ) {
        prev = q->next;
        free(q);
        q = prev;
    }
}

Q *push(Q *q, int x) {
    Q *node, *prev = NULL;
    for (node = q; node; node = node->next) {
        prev = node;
    }
    Q *this = one(x);
    printf("prev=%p\n", prev);
    if (prev == NULL) {
        return this;
    } else {
        prev->next = this;
        return q;
    }
}

Q *pop(Q *q, int *px) {
    if (q == NULL) {
        return NULL;
    }
    *px = q->x;
    Q *r = q->next;
    free(q);
    return r;
}

void show(Q *q) {
    printf("q=%p\n", q);
    for (; q; q = q->next) {
        printf("%d, ", q->x);
    }
    printf("\n");
}

int main() {
    int a[] = {1, 2, 3, 4, 7, 8, 9, 10};
    // int a[] = {};
    Q* q = build(a, sizeof(a) / sizeof(a[0]));
    show(q);
    q = push(q, 5);
    show(q);
    int x;
    q = pop(q, &x);
    printf("x=%d\n", x);
    q = pop(q, &x);
    printf("x=%d\n", x);
    show(q);
    dtor(q);
    return 0;
}
