#include <stdio.h>
int func2const(int n, const char *s[]) {
    int i;
    if (n == 0) {
        printf("nil\n");
    }
    for (i = 0; i != n; i++) {
        printf("s[%d]=%s\n", i, s[i]);
    }
}

int func2(int n, char *s[]) {
    int i;
    if (n == 0) {
        printf("nil\n");
    }
    for (i = 0; i != n; i++) {
        printf("s[%d]=%s\n", i, s[i]);
    }
}

int func(char *s) {
    printf("func at %s\n", s);
    return 0;
}

int main() {
    // Wrong using, s point to const char *
    // char *s = "hello world";
    char s[] = "hello world";
    func(s);

    // Wrong using,
    // char *sa[] = {"hello", "world"};
    const char *csa[] = {"hello", "world"};
    func2const(2, csa);

    // force cast to silent G++'s warning
    char *sa[] = {(char *)"hello", (char *)"world"};
    func2(2, sa);
    return 0;
}
