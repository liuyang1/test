/** TODO */
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

bool isSubsequence(char *s, char *sub) {
    for (; *s != '\0'; s++) {
        if (*s == *sub) {
            sub++;
        }
    }
    return *sub == '\0';
}

int sign(int a) {
    return (a > 0) - (a < 0);
}

int compare(char *a, char *b) {
    int s = sign(strlen(a) - strlen(b));
    if (s == 0) {
        s = strcmp(a, b);
    }
    return s;
}

char *empty = "";
char *find_longest_word(char *s, char **d, int dSize) {
    char **ret = &empty;
    for (int i = 0; i != dSize; i++) {
        if (isSubsequence(s, d[i]) && compare(d[i], *ret) > 0) {
            ret = &d[i];
        }
    }
    return *ret;
}

int test() {
    // char *string = "abpcplea";
    char *string = "abp";
    char *__dict[] = {"ale", "apple", "monkey", "plea", "bpple"};
    char *r = find_longest_word(string, __dict, sizeof(__dict) / sizeof(__dict[0]));
    printf("%s\n", r);
    return 0;
}

int main() {
    test();
    return 0;
}
