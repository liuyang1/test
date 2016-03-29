#include "leet.h"

char *dropWhile(char *s, bool (*isCond)(char)) {
    if (s == NULL) {
        return NULL;
    }
    for (; *s != '\0' && isCond(*s); s++) {
    }
    return s;
}

bool oneOf(char c, char *s) {
    for (; *s != '\0'; s++) {
        if (c == *s) {
            return true;
        }
    }
    return false;
}

char *oneMore(char *s, bool (*isCond)(char)) {
    bool first = false;
    for (; *s != '\0'; s++) {
        if (isCond(*s)) {
            first = true;
        } else {
            break;
        }
    }
    return first ? s : NULL;
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isSpace(char c) {
    return c == ' ';
}

char *isNum(char *s) {
    if (oneOf(*s, "+-")) {
        s++;
    }
    char *s1 = NULL, *s2 = NULL;
    bool zheng = false, frac = false;
    s1 = dropWhile(s, isDigit);
    zheng = s1 != s;
    s = s1;
    if (*s == '.') {
        s++;
        s2 = dropWhile(s, isDigit);
        frac = s2 != s;
        s = s2;
    }
    return zheng || frac ? s : NULL;
}

char *isExp(char *s) {
    if (oneOf(*s, "+-")) {
        s++;
    }
    return oneMore(s, isDigit);
}

bool isNumber(char *s) {
    s = dropWhile(s, isSpace);
    s = isNum(s);
    if (s != NULL && *s == "e") {
        s++;
        s = isExp(s);
    }
    s = dropWhile(s, isSpace);
    return s != NULL && *s == '\0';
}

#define CASE(s, e) {bool r = isNumber(s); \
                    printf("[%s]\t%s ?= %s %s\n", \
                           s, SBOOL(r), SBOOL(e), expect(r == e)); }

int main() {
    CASE("0", true);
    CASE("123", true);
    CASE(".1", true);
    CASE(".0", true);
    CASE("1.", true);
    CASE("1.2", true);
    CASE(" 0.1", true);
    CASE(" 0.1 ", true);
    CASE("+.1", true);
    CASE("2e10", true);
    CASE(".1", true);
    CASE("+1", true);
    CASE("-1", true);
    CASE("+1e-1", true);
    CASE("abc", false);
    CASE("1 a", false);
    CASE("+1.1e-1.1", false);
    CASE(".1e.1", false);
    CASE("1.e1.", false);
    CASE("2e2.", false);
    CASE("0xab", false);
    CASE("  ", false);
    CASE("+-123", false);
    CASE(".", false);
    CASE(" 0..1 ", false);
    CASE(".1.1", false);
    CASE("2ee10", false);
    CASE("1E10", false);
    CASE("e9", false);
    return 0;
}
