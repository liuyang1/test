#include "leet.h"
char *dropWhile(char *s, bool (*isCond)(char)) {
    if (s == NULL) {
        return NULL;
    }
    for (; *s != '\0' && isCond(*s); s++) {
    };
    return s;
}
char *all(char *s, bool (*isCond)(char)) {
    for (; *s != '\0'; s++) {
        if (!isCond(*s)) {
            printf("c=[%c] is not cond\n", *s);
            return NULL;
        }
    }
    return s;
}
bool oneOf(char c, char* s) {
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
char *eatOne(char *s, bool (*isCond)(char)) {
    if (isCond(*s)) {
        s++;
    }
    return s;
}

bool isDigit(char c) { return c >= '0' && c <= '9'; }
bool isSpace(char c) { return c == ' '; }
bool isSign(char c) { return c == '+' || c == '-'; }
bool isDot(char c) { return c == '.'; }
bool isExp(char c) { return c == 'e'; }

char *isNum(char *s) {
    if (oneOf(*s, "+-")) {
        s++;
    }
    switch(*s) {
        case '\0': return NULL;
        case '.': return oneMore(s + 1, isDigit);
        default:
                  s = dropWhile(s, isDigit);
                  printf("after drop c=[%c]\n", *s);
                  switch(*s) {
                      case '\0': return s;
                      case '.': return dropWhile(s + 1, isDigit);
                      default: return NULL;
                  }
    }
}
bool isNumber(char* s) {
    s = dropWhile(s, isSpace);
    s = isNum(s);
    if (s != NULL && oneOf(*s, "e")) {
        s++;
        s = isNum(s);
        LOG("exp return s=%p c=[%c]\n", s, s?*s:'x');
    }
    s = dropWhile(s, isSpace);
    if (s != NULL) {
        LOG("c=[%c]\n", *s);
    }
    return s != NULL && *s == '\0';
}

#define CASE(s, e) {bool r = isNumber(s); \
    printf("[%s]\t%s ?= %s %s\n", s, SBOOL(r), SBOOL(e), expect(r == e)); }

int main() {
    CASE("  ", false);
    CASE("0", true);
    CASE("123", true);
    CASE("+-123", false);
    CASE(".1", true);
    CASE(".0", true);
    CASE(".", false);
    CASE("1.", true);
    CASE(" 0.1", true);
    CASE(" 0.1 ", true);
    CASE(" 0..1 ", false);
    CASE(".1.1", false);
    CASE("+.1", true);
    CASE("abc", false);
    CASE("1 a", false);
    CASE("2e10", true);
    CASE("2ee10", true);
    CASE("1E10", false);
    CASE(".1", true);
    CASE("+1", true);
    CASE("-1", true);
    CASE("+1e-1", true);
    CASE("+1.1e-1.1", true);
    CASE("0xab", false);
    return 0;
}
