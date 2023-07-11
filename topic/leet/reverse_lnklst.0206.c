#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *reverse(struct ListNode *head) {
    struct ListNode *c, *next, *prev;
    for (c = head, prev = NULL; c != NULL; prev = c, c = next) {
        next = c->next;
        c->next = prev;
    }
    return prev;
}

struct ListNode *ctor(int *a, size_t n) {
    size_t i;
    struct ListNode *next, *p;
    for (i = n, p = next = NULL; i != 0; ) {
        p = malloc(sizeof(struct ListNode));
        p->val = a[--i];
        p->next = next;
        next = p;
    }
    return p;
}

void dtor(struct ListNode *head) {
    struct ListNode *next;
    for (; head != NULL; head = next) {
        next = head->next;
        free(head);
    }
}

void show(struct ListNode *head) {
    for (; head != NULL; head = head->next) {
        printf("%d, ", head->val);
    }
    printf("\n");
}

int main() {
    int a[] = {0, 1, 2, 3, 4};
    struct ListNode *h = ctor(a, sizeof(a) / sizeof(a[0]));
    show(h);
    h = reverse(h);
    show(h);
    dtor(h);
    return 0;
}
