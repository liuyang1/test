#include <stdio.h>
#include <stdlib.h>
void show_arr(int *a, int l) {
    int i;
    for (i = 0; i != l; i++) {
        printf("%d,", a[i]);
    }
    printf("\n");
}
int main() {
    int len = 1000 * 1000 + 1;
    int *a = malloc(sizeof(int) * len), s = 0;
    int lena, lenb, i;
    char c;
    for (i = lena = 0; (c = getchar()) != '\n'; i++, lena++) {
        s += c == '1';
        a[i] = s;
    }
    show_arr(a, lena);
    s = 0;
    for (lenb = 0; (c = getchar()) != '\n'; lenb++) {
        s += c == '1';
    }
    s %= 2;
    printf("%d\n", s);
    int r;
    for (i = r = 0; i != lena - lenb; i++) {
        r += ((a[i + lenb] - a[i]) % 2) == s;
    }
    printf("%d\n", r);
    free(a);
    return 0;
}
