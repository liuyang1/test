char *convertToTitle(int n) {
    int l, ori = n;
    for (l = 1; n != 0; n = (n - 1) / 26, l++) {
    }
    char *r = malloc(sizeof(char) * (l));
    int i;
    for (i = 0, n = ori; i != l - 1; i++, n = (n - 1) / 26) {
        r[l - 2 - i] = 'A' + (n - 1) % 26;
    }
    r[l - 1] = '\0';
    return r;
}

#include <stdio.h>
#define CASE(n, e) {char *s = convertToTitle(n); printf("%d %s ?= %s %s\n", \
                                                     n, s, e, strcmp(s, e) == 0 ? "PASS" : "fail"); }
int main()
{
    CASE(0, "");
    CASE(1, "A");
    CASE(25, "Y");
    CASE(26, "Z");
    CASE(27, "AA");
    CASE(100, "CV");
    CASE(702, "ZZ");
    CASE(703, "AAA");
    CASE(731, "ABC");
    CASE(16900, "XYZ");
    return 0;
}
