#include <string.h>
#include <stdio.h>
char * showPage(int i, int n, char *s, int len) {
    if (i <= n) {
        snprintf(s, len, "%d", i);
    } else {
        snprintf(s, len, "Blank");
    }
    return s;
}

#define DUM "Blank"
#define LEN 16

int booklet(int n) {
    printf("Printing order for %d pages:\n", n);
    int sheet = n / 4 + (n % 4 != 0);
    int i;
    for (i = 0; i != sheet; i++) {
        char s0[LEN], s1[LEN];
        showPage(4 * sheet - 2 * i, n, s0, LEN);
        showPage(2 * i + 1, n, s1, LEN);
        printf("Sheet %d, front: %s, %s\n", i + 1, s0, s1);

        showPage(2 * i + 2, n, s0, LEN);
        showPage(4 * sheet - 1 - 2 * i, n, s1, LEN);
        if (strcmp(s0, DUM) == 0 && strcmp(s1, DUM) == 0) {
            continue;
        }
        printf("Sheet %d, back : %s, %s\n", i + 1, s0, s1);
    }
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n == 0) {
            break;
        }
        booklet(n);
    }
    return 0;
}
