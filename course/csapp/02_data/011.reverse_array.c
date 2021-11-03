/**
 * Practice Problem 2.10, 2.11
 */
#include <stdio.h>
#include <assert.h>

void show_array(int a[], int cnt)
{
    int i;

    for (i = 0; i != cnt; i++) {
        printf("%x, ", a[i]);
    }
    printf("\n");
}

void swap(int *a, int *b)
{
    assert(a != b); /** reset to zero iff a == b */
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void reverse_array(int a[], int cnt)
{
    int i, j;

    for (i = 0, j = cnt - 1;
         // i <= j;
         i < j; /** avoid swap on same address */
         i++, j--) {
        swap(a + i, a + j);
    }
}

int main()
{
    int a = 3, b = 5;

    swap(&a, &b);
    printf("a=%x b=%x\n", a, b);
    /*
     * swap(&a, &a);
     * printf("a=%x\n", a);
     */
    int a0[] = { 1, 2, 3, 4 };
    int cnt0 = sizeof(a0) / sizeof(a0[0]);
    reverse_array(a0, cnt0);
    show_array(a0, cnt0);

    int a1[] = { 1, 2, 3, 4, 5 };
    int cnt1 = sizeof(a1) / sizeof(a1[0]);
    reverse_array(a1, cnt1);
    show_array(a1, cnt1);
    return 0;
}
