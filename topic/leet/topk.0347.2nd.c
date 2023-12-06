#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *arr;
    size_t len;
    size_t cap;
} pq;

void pq_show(pq *q)
{
    printf("q=%p arr=%p len=%zu cap=%zu ",
           q, q->arr, q->len, q->cap);
    size_t i;
    for (i = 1; i != q->len; i++) {
        printf("%d, ", q->arr[i]);
    }
    printf("\n");
}

pq *pq_create(int k)
{
    pq *q = malloc(sizeof(pq));
    q->arr = malloc(sizeof(int) * (k + 1));
    q->cap = k + 1;
    q->len = 1;
}

void pq_destroy(pq *q)
{
    free(q->arr);
    free(q);
}

int pq_less(pq *q, size_t i, size_t j)
{
    return q->arr[i] - q->arr[j] < 0;
}

void pq_swap(pq *q, size_t i, size_t j)
{
    int x = q->arr[i];
    q->arr[i] = q->arr[j];
    q->arr[j] = x;
}

void pq_swim(pq *q, size_t i)
{
    while (i != 1 && pq_less(q, i, i / 2)) {
        pq_swap(q, i, i / 2);
        i /= 2;
    }
}

void pq_sink(pq *q)
{
    size_t i;
    for (i = 1; i < q->len;) {
        size_t child = 2 * i;
        if (child + 1 < q->len && pq_less(q, child + 1, child)) {
            child++;
        }
        if (child + 0 < q->len && pq_less(q, child, i)) {
            pq_swap(q, i, child);
            i = child;
        } else {
            break;
        }
    }
}

void pq_insert(pq *q, int x)
{
    q->arr[q->len] = x;
    pq_swim(q, q->len);
    q->len++;

    // if (q->len + 1 == q->cap) {
    //     pq_pop(q); // limit capcity
    // }
}

int pq_pop(pq *q)
{
    int x = q->arr[1];
    q->arr[1] = q->arr[q->len - 1];
    q->len--;
    pq_sink(q);
    return x;
}

int main()
{
    int a[] = {10, 16, 14, 4, 1,
               13, 15, 7, 5, 6};
    pq *q = pq_create(20);
    size_t i;
    for (i = 0; i != sizeof(a) / sizeof(a[0]); i++) {
        pq_insert(q, a[i]);
        pq_show(q);
    }

    for (i = 0; i != sizeof(a) / sizeof(a[0]); i++) {
         pq_pop(q);
         pq_show(q);
    }
    pq_destroy(q);
    return 0;
}
