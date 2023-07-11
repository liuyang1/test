#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *build_lnklst(int *a, size_t n)
{
    struct ListNode *last;
    struct ListNode *node;
    size_t i;
    for (i = n, last = NULL; i > 0; last = node) {
        node = malloc(sizeof(struct ListNode));
        node->val = a[--i];
        node->next = last;
    }
    return last;
}

void show_lnklst(struct ListNode *n)
{
    for (; n != NULL; n = n->next) {
        printf("%d, ", n->val);
    }
    printf("\n");
}

bool eq_lnklst(struct ListNode *a, struct ListNode *b)
{
    for (; a != NULL && b != NULL; a = a->next, b = b->next) {
        if (a->val != b->val) {
            return false;
        }
    }
    return a == NULL && b == NULL;
}

void free_lnklst(struct ListNode *a)
{
    struct ListNode *nxt;
    for (; a != NULL; a = nxt) {
        nxt = a->next;
        free(a);
    }
}

#define UNIT(a, v, e) {                                                     \
        struct ListNode *lst_a = build_lnklst(a, sizeof(a) / sizeof(a[0])); \
        struct ListNode *lst_e = build_lnklst(e, sizeof(e) / sizeof(e[0])); \
        struct ListNode *lst_r = remove_element(lst_a, v);                  \
        if (eq_lnklst(lst_r, lst_e) != true) {                              \
            printf("fail when remove_element\n");                           \
            show_lnklst(lst_r);                                             \
            show_lnklst(lst_e);                                             \
            return -1;                                                      \
        }                                                                   \
        free_lnklst(lst_e);                                                 \
        free_lnklst(lst_r);                                                 \
}

struct ListNode *remove_element(struct ListNode *head, int val)
{
    struct ListNode *p, *next, *prev;
    for (p = head; p != NULL; p = next) {
        next = p->next;
        if (p->val != val) {
            prev = p; // other element, just interate
        } else if (p == head) {
            head = next; // remove head, need update head pointer
            free(p);
        } else {
            prev->next = next; // remove element
            free(p);
        }
    }
    return head;
}

int main()
{
    UNIT(((int[]) {0, 0, 1, 2}), 0, ((int[]) {1, 2})); // first
    UNIT(((int[]) {0, 1, 1, 2}), 1, ((int[]) {0, 2})); // middle
    UNIT(((int[]) {0, 1, 2, 2}), 2, ((int[]) {0, 1})); // last

    UNIT(((int[]) {0, 1, 2, 2}), 4, ((int[]) {0, 1, 2, 2})); // non-exist
    UNIT(((int[]) {0, 1, 2, 0, 2}), 0, ((int[]) {1, 2, 2})); // duplicate

    UNIT(((int[]) {}), 2, ((int[]) {})); // empty list

    return 0;
}
