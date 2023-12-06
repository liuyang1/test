#include <stdio.h>
#include <stdbool.h>

int strprefixeq_y(const char *s1, const char *s2) {
    for (; *s1 != '\0' && *s2 != '\0'; s1++, s2++) {
        if (*s1 != *s2) {
            return false;
        }
    }
    return true; // This is different with strcmp
}

char *strstr_y(const char *haystack, const char *needle) {
    const char *p, n;
    for (p = haystack; *p != '\0'; p++) {
        if (strprefixeq_y(p, needle)) {
            return (char *)p;
        }
    }
    return NULL;
}

int main() {
    char needle[] = "0001";
    char haystack[] = "000010001010001";

    char *p = haystack;
    while (1) {
        p = strstr_y(p, needle);
        if (p == NULL) {
            break;
        }
        printf("%s\n", p);
        p++;
    }

    return 0;
}
