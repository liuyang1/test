#include "kr.h"
#define STRLEN      1024

// 5.3
char *strcat_p(char *dst, const char *src) {
    char *r = dst;
    for (; *dst != '\0'; dst++) {
    }
    for (; *src != '\0'; *dst++ = *src++) {
    }
    *dst = '\0';
    return r;
}

int test_strcat() {
#undef CASE
#define CASE(a, b) {char t0[STRLEN], t1[STRLEN];                              \
                    strcpy(t0, a); strcpy(t1, a);                             \
                    strcat_p(t0, b); strcat(t1, b);                           \
                    printf("strcat(\"%s\", \"%s\") => \"%s\" ?= \"%s\" %s\n", \
                           a, b, t0, t1, expect(strcmp(t0, t1) == 0)); }
    CASE("hello", "world!");
    CASE("hello", "");
    CASE("", "hello");
    CASE("", "");
    return 0;
}

char *strncat_p(char *dst, const char *src, size_t n) {
    char *r = dst;
    for (; *dst != '\0'; dst++) {
    }
    for (; *src != 0 && n != 0; *dst++ = *src++, n--) {
    }
    for (; n != 0; *dst++ = '\0', n--) {
    }
    return r;
}

int test_strncat() {
#undef CASE
#define CASE(a, b, n) {char t0[STRLEN] = {0}, t1[STRLEN] = {0}; \
                       strcpy(t0, a); strcpy(t1, a);            \
                       strncat_p(t0, b, n); strncat(t1, b, n);  \
                       printf("strncat(\"%s\", \"%s\", %d) => " \
                              "\"%s\" ?= \"%s\" %s\n",          \
                              a, b, n, t0, t1,                  \
                              expect(strcmp(t0, t1) == 0)); }
    CASE("hello", "world!", 4);
    CASE("hello", "world!", 7);
    CASE("hello", "world!", 10);
    CASE("hello", "", 0);
    CASE("hello", "", 1);
    CASE("hello", "", 2);
    CASE("", "hello", 4);
    CASE("", "hello", 5);
    CASE("", "hello", 6);
    CASE("", "", 1);
    return 0;
}

// 5.4
bool strend(const char *s, const char *t) {
    int l0 = strlen(s), l1 = strlen(t);
    if (l0 < l1) {
        return false;
    }
    if (l1 == 0) {
        return true;
    }
    const char *ps = s + l0, *pt = t + l1;
    while (pt >= t) {
        if (*ps-- != *pt--) {
            return false;
        }
    }
    return true;
}

int test_strend() {
#undef CASE
#define CASE(a, b, e) {bool r = strend(a, b);                    \
                       printf("strend(%s, %s) => %d ?= %d %s\n", \
                              a, b, r, e, expect(r == e)); }

    CASE("", "", true);
    CASE("hello", "", true);
    CASE("hello", "llo", true);
    CASE("hello", "hello", true);
    CASE("hello", "alo", false);
    CASE("", "ab", false);
    CASE("hello", "ahello", false);
    return 0;
}

// 5.5
char *strncpy_p(char *dst, const char *src, size_t n) {
    *dst = '\0';
    return strncat_p(dst, src, n);
}

int test_strncpy() {
#undef CASE
#define CASE(b, n) {char t0[STRLEN] = {0}, t1[STRLEN] = {0};               \
                    strncpy_p(t0, b, n); strncpy(t1, b, n);                \
                    printf("strncpy(\"%s\", %d) => \"%s\" ?= \"%s\" %s\n", \
                           b, n, t0, t1,                                   \
                           expect(strcmp(t0, t1) == 0)); }
    CASE("world!", 4);
    CASE("world!", 7);
    CASE("world!", 10);
    CASE("", 0);
    CASE("", 1);
    CASE("", 2);
    CASE("hello", 4);
    CASE("hello", 5);
    CASE("hello", 6);
    CASE("", 1);
    return 0;
}

int strncmp_p(const char *s0, const char *s1, size_t n) {
    for (;
         n != 0 && *s0 != '\0' && *s1 != '\0' && *s0 == *s1;
         n--, s0++, s1++) {
    }
    return n == 0 ? 0 : *s0 - *s1;
}

int strcasecmp(const char *s0, const char *s1) {
    for (;
         *s0 != '\0' && *s1 != '\0' && tolower(*s0) == tolower(*s1);
         s0++, s1++) {
    }
    return *s0 - *s1;
}

int test_strncmp() {
#undef CASE
#define CASE(a, b, n) {bool r = strncmp_p(a, b, n);                  \
                       bool e = strncmp(a, b, n);                    \
                       printf("strncmp(%s, %s, %d) = %d ?= %d %s\n", \
                              a, b, n, r, e, expect(r == e)); }
    CASE("hello", "hillo", 1);
    CASE("hello", "hillo", 2);
    CASE("hello", "hillo", 3);
    CASE("hello", "helloa", 5);
    CASE("hello", "helloa", 6);
    return 0;
}

char *strdup_p(const char *s) {
    char *q, *p = malloc(sizeof(char) * (strlen(s) + 1));
    if (p == NULL) {
        return NULL;
    }
    for (q = p; *s != '\0'; q++, s++) {
        *q = *s;
    }
    *q = '\0';
    return p;
}

char *strndup_p(const char *s, size_t n) {
    size_t slen = strlen(s);
    int len = slen < n ? slen : n;
    char *q, *p = malloc(sizeof(char) * (len + 1));
    if (p == NULL) {
        return NULL;
    }
    for (q = p; *s != '\0' && q - p != len; q++, s++) {
        *q = *s;
    }
    *q = '\0';
    return p;
}

bool unit_strdup(const char *s) {
    char *r = strdup_p(s);
    char *e = strdup(s);
    bool ret = strcmp(r, e) == 0;
    printf("strdup(%s) = %s ? = %s %s\n", s, r, e, expect(ret));
    free(r);
    free(e);
    return ret;
}

bool unit_strndup(const char *s, int n) {
    char *r = strndup_p(s, n);
    char *e = strndup(s, n);
    bool ret = strcmp(r, e) == 0;
    printf("strndup(%s, %d) = %s ? %s %s\n", s, n, r, e, expect(ret));
    free(r);
    free(e);
    return ret;
}

int test_strdup() {
    unit_strdup("Hello, World!");
    unit_strdup("");
    return 0;
}

int test_strndup() {
    const char s[] = "Hello, World!";
    int len = strlen(s);
    int i;
    for (i = 0; i != len + 1; i++) {
        unit_strndup("Hello, World!", i);
    }
    return 0;
}

// locate char in string
char *strchr(const char *s, int c) {
    for (; *s != '\0'; s++) {
        if (*s == c) {
            return (char *)s;
        }
    }
    return NULL;
}

char *strrchr(const char *s, int c) {
    const char *p = s + strlen(s);
    for (; p > s; p--) {
        if (*p == c) {
            return (char *)p;
        }
    }
    return NULL;
}

int main() {
    test_strcat();
    test_strncat();
    test_strend();
    test_strncpy();
    test_strncmp();
    test_strdup();
    test_strndup();
    return 0;
}
