#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef int T;

typedef struct {
    T *v;
    size_t len;
    size_t cap;
} Heap;

Heap *heap_create(size_t cap)
{
    Heap *h = malloc(sizeof(Heap));
    h->v = malloc(sizeof(T) * (cap + 1));
    h->cap = cap + 0;
    h->len = 0; // h->v[0] is dummy
    return h;
}

size_t heap_root()
{
    return 1;
}

size_t heap_left(size_t i)
{
    return 2 * i;
}

size_t heap_right(size_t i)
{
    return 2 * i + 1;
}

size_t heap_parent(size_t i)
{
    return i / 2;
}

size_t heap_last(Heap *h)
{
    return h->len;
}

int cmp(T *a, T *b)
{
    return *a - *b;
}

void exchange(T *a, T *b)
{
    T t;
    t = *a;
    *a = *b;
    *b = t;
}

bool heap_empty(Heap *h)
{
    return h->len > 1;
}

void heap_show(Heap *h)
{
    printf("h=%p cap=%zu len=%zu\n", h, h->cap, h->len);
    size_t i;
    for (i = heap_root(); i != h->cap + 1; i++) {
        printf("%d, ", h->v[i]);
    }
    printf("\n");
}

T heap_pop(Heap *h)
{
    if (h->len == 1) {
        assert(0);
    }
    T r = h->v[heap_root()]; // root
    h->v[heap_root()] = h->v[heap_last(h)];
    h->v[heap_last(h)] = 0; // for debug
    h->len--;
    // sink
    size_t i;
    for (i = 1; i < h->len;) {
        T *root = h->v + i;
        printf("i=%zu len=%zu\n", i, h->len);
        bool left = 2 * i < h->len && cmp(root, root + i) > 0;
        bool right = 2 * i + 1 < h->len && cmp(root, root + i + 1) > 0;
        if (left && right) {
            if (cmp(root + i, root + i + 1) > 0) {
                left = false; right = true;
            } else {
                left = true; right = false;
            }
        }
        if (left) {
            printf("exchange idx %zu left=%zu, val=%d %d\n",
                   i, 2 * i, h->v[i], h->v[2 * i]);
            exchange(root, root + i);
            i = 2 * i;
        } else if (right) {
            printf("exchange idx %zu right=%zu, val=%d %d\n",
                   i, 2 * i + 1, h->v[i], h->v[2 * i + 1]);
            exchange(root, root + i + 1);
            i = 2 * i + 1;
        } else {
            break;
        }
    }
    return r;
}

void heap_insert(Heap *h, T *p)
{
    if (h->len == h->cap) {
        if (*p < h->v[heap_root()]) {
            return; // not need insert
        }
        heap_pop(h);
        heap_show(h);
    }
    h->len++;
    size_t i = heap_last(h);
    h->v[i] = *p;
    if (i == heap_root()) {
        return;
    }
    for (; i != 0;) {
        if (cmp(&h->v[i], &h->v[heap_parent(i)]) > 0) {
            // do nothing
            break;
        } else {
            exchange(&h->v[i], &h->v[heap_parent(i)]);
            i /= 2;
        }
    }
}

int main()
{
    int a[] = {10, 16, 14, 4, 1,
               13, 15, 7, 5, 6};
    Heap *h = heap_create(5);
    size_t i;
    for (i = 0; i != sizeof(a) / sizeof(a[0]); i++) {
        heap_insert(h, &a[i]);
        heap_show(h);
    }

    while (!heap_empty(h)) {
        printf("%d, ", heap_pop(h));
    }
    return 0;
}
