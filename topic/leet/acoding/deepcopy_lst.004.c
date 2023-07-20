#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    void **keys;
    void **vals;
    size_t cap;
} Map;

size_t hash(Map *m, void *key) {
    return (size_t)key % m->cap;
}

Map *map_ctor(size_t cap) {
    Map *m = malloc(sizeof(Map));
    m->cap = cap;
    size_t sz = sizeof(void *) * cap;
    m->keys = malloc(sz);
    m->vals = malloc(sz);
    memset(m->keys, 0x00, sz);
    return m;
}

void map_dtor(Map *m) {
    free(m->keys);
    free(m->vals);
    free(m);
}

int map_insert(Map *m, void *k, void *v) {
    size_t i = hash(m, k);
    for (; i != m->cap; i++) {
        if (m->keys[i] == NULL) {
            break;
        }
    }
    if (i == m->cap) {
        printf("map cap is not enough %zu\n", m->cap);
        return -1;
    }
    m->keys[i] = k;
    m->vals[i] = v;
    return 0;
}

void *map_map(Map *m, void *k) {
    size_t i = hash(m, k);
    for (; i != m->cap; i++) {
        if (m->keys[i] == k) {
            return m->vals[i];
        }
    }
    return NULL;
}

typedef struct Node {
    int x;
    struct Node *next;
    struct Node *arb;
} Node;

void show(Node *a);
Node *deepcopy(Node *a) {
    Node *heada = a;
    Map *m = map_ctor(97);
    Node *head, *prev;
    for (head = prev = NULL; a != NULL; a = a->next) {
        Node *n = malloc(sizeof(Node));
        n->x = a->x;
        n->next = NULL;
        n->arb = NULL;
        map_insert(m, a, n);

        if (head == NULL) {
            head = prev = n;
        } else {
            prev->next = n;
            prev = n;
        }
    }

    Node *b = head;
    show(b);
    show(heada);
    for (a = heada; a != NULL; a = a->next, b = b->next) {
        if (a->arb != NULL) {
            b->arb = map_map(m, a->arb);
        }
    }
    map_dtor(m);

    return head;
}

void show(Node *a) {
    for (; a != NULL; a = a->next) {
        printf("%d", a->x);
        if (a->arb) {
            printf("(%d)", a->arb->x);
        }
        printf(", ");
    }
    printf("\n");
}

Node *build(int *a, size_t n) {
    Node *head, *prev;
    size_t i;
    for (i = 0, head = prev = NULL; i != n; i++) {
        Node *n = malloc(sizeof(Node));
        n->x = a[i];
        n->next = NULL;
        n->arb = NULL;

        if (head == NULL) {
            head = prev = n;
        } else {
            prev->next = n;
            prev = n;
        }
    }
    return head;
}

void dtor(Node *a) {
    Node *next;
    for (; a != NULL; a = next) {
        next = a->next;
        free(a);
    }
}

int main() {
    int a[] = {1, 2, 3, 4, 5};
    Node *l = build(a, sizeof(a) / sizeof(a[0]));
    l->next->next->arb = l->next;
    l->next->arb = l->next->next;
    show(l);

    Node *l1 = deepcopy(l);
    show(l1);

    dtor(l);
    dtor(l1);
    return 0;
}

