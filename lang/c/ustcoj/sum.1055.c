/** also for 1036, 1050
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse(char *a) {
    int n = strlen(a), i;
    for (i = 0; i != n / 2; i++) {
        char c = a[i];
        a[i] = a[n - i - 1];
        a[n - i - 1] = c;
    }
}
int max(int a, int b) {
    return a > b ? a : b;
}
char digit2char(char c) { return c + '0'; }
char char2digit(char c) { return c - '0'; }
void add_in(char *c, char *a, char *b) {
    int na = strlen(a), nb = strlen(b);
    char carry = 0;
    int i;
    for (i = 0; i < na || i < nb; i++) {
        int va = i < na ? char2digit(a[i]) : 0;
        int vb = i < nb ? char2digit(b[i]) : 0;
        int v = va + vb + carry;
        c[i] = digit2char(v % 10);
        carry = v / 10;
    }
    if (carry != 0) {
        c[i] = digit2char(carry);
        i++;
    }
    c[i] = '\0';
}

char *add(char *a, char *b) {
    char *na = strdup(a);
    char *nb = strdup(b);
    reverse(na);
    reverse(nb);
    char *c = malloc(sizeof(char) * (max(strlen(a), strlen(b)) + 2));
    add_in(c, na, nb);
    reverse(c);
    free(na);
    free(nb);
    return c;
}

#define LEN     1024
int main() {
    char a[LEN], b[LEN];
    while (scanf("%s%s", a, b) == 2) {
        char *c = add(a, b);
        printf("%s\n", c);
        free(c);
    }
    return 0;
}
