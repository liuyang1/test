#include <string.h>

int cmpVer(char *s0, char *s1)
{
    if (s0 != NULL && *s0 == '0') {
        s0 = NULL;
    }
    if (s1 != NULL && *s1 == '0') {
        s1 = NULL;
    }
    if (s0 == NULL && s1 == NULL) {
        return 0;
    }
    if (s0 == NULL) {
        return -1;
    }
    if (s1 == NULL) {
        return 1;
    }
    int l0 = strlen(s0), l1 = strlen(s1);
    if (l0 == l1) {
        int r = strcmp(s0, s1);
        return (r > 0) - (r < 0);
    }
    return l0 > l1 ? 1 : -1;
}

char *lastNotZero(char *s)
{
    if (s == NULL) {
        return NULL;
    }
    while (*s == '0') {
        s++;
    }
    if (*s == '\0') {
        return s - 1;
    } else {
        return s;
    }
}

char *splitVerion(char *s)
{
    if (s == NULL) {
        return NULL;
    }
    char *p = strchr(s, '.');
    if (p != NULL) {
        *p = '\0';
        return p + 1;
    }
    return NULL;
}

int compareVersion(char *s0, char *s1)
{
    if (s0 == NULL && s1 == NULL) {
        return 0;
    }
    char *p0 = splitVerion(s0);
    char *p1 = splitVerion(s1);
    s0 = lastNotZero(s0);
    s1 = lastNotZero(s1);
    // printf("%s %s vs. %s %s\n", s0, p0, s1, p1);
    int r = cmpVer(s0, s1);
    if (r != 0) {
        return r > 0 ? 1 : -1;
    }
    r = compareVersion(p0, p1);
    return (r > 0) - (r < 0);
}

#include <stdio.h>
#define CASE(s0, s1, e)        {char p0[] = s0; char p1[] = s1; printf("%s %s %s\n", s0, s1, \
                                                                       compareVersion(p0, p1) == e ? \
                                                                       "PASS" : "fail"); }
int main()
{
    CASE("10.1", "0.2", 1);
    CASE("1000", "100", 1);
    CASE("1.1", "1.2", -1);
    CASE("1.1", "0.2", 1);
    CASE("0", "0", 0);
    CASE("1", "0", 1);
    CASE("0", "1", -1);
    CASE("1", "1.1", -1);
    CASE("01", "1", 0);
    CASE("00", "0.1", -1);
    CASE("1.0", "1.2", -1);
    CASE("1.2", "1.10", -1);
    CASE("0.1", "0.0.1", 1);
    CASE("1.0", "1", 0);
    return 0;
}
