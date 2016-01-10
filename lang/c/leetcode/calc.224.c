#include "leet.h"

/* only support + -
 * not support negative number
 * support parenthesis
 * support empty
 */
static inline char *skipWhitespace(char *s) {
    for (; *s == ' '; s++) {
    }
    return s;
}
static inline char *rmParen(char *s) {
    int len = strlen(s);
    char *p0 = skipWhitespace(s);
    if (*p0 != '(') {
        return p0;
    }
    char *p1;
    int paren;
    for (p1 = p0, paren = 0; *p1 != '\0'; p1++) {
        if (*p1 == '(') {
            paren++;
        } else if (*p1 == ')') {
            paren--;
            if (paren == 0) {
                break;
            }
        }
    }
    if (paren == 0 && *p1 == ')') {
        char *p2;
        for (p2 = p1 + 1; *p2 == ' '; p2++) {
        }
        LOG("find around paren %p [%c]\n", p0, *p0);
        if (*p2 == '\0') {
            LOG(">>> find around paren %p\n", p0);
            *p1 = '\0';
            return p0 + 1;
        }
    }
    return s;
}
static inline char *findOp(char *s) {
    int len = strlen(s);
    char *p;
    int paren;
    for (p = s + len - 1, paren = 0; p != s - 1; p--) {
        if (paren == 0 && (*p == '+' || *p == '-')) {
            return p;
        } else if (*p == ')') {
            paren++;
        } else if (*p == '(') {
            paren--;
        }
    }
    return NULL;
}

int calculateI(char* s) {
    LOG("calc [%s]\n", s);
    s = rmParen(s);
    LOG("per [%s]\n", s);
    char *p, cop, *op;
    char *lefts, *rights;
    int left, right;
    op = findOp(s);
    LOG("s=%p s=%s op=%p %c\n", s, s, op, op != NULL ? *op : ' ');
    if (op == NULL) {
        return atoi(s);
    }
    cop = *op;
    *op = '\0';
    lefts = s;

    left = calculateI(lefts);
    rights = op + 1;
    right = calculateI(rights);

    switch (cop) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        default:
            LOG("op=[%c]\n", cop);
            return 0;
    }
}

int calculate(char *s) {
    char *p = strdup(s);
    int r = calculateI(p);
    free(p);
    return r;
}

int main() {
#define CASE(s, e) {int r = calculate(s); \
    printf("[%.20s] %d ?= %d %s\n", s, r, e, expect(r == e));}
    CASE("1+2", 3);
    CASE("111-222", -111);
    CASE("111", 111);
    CASE("111 + 222", 333);
    CASE(" 111 + 222 ", 333);
    CASE(" 111 + 222 + 333", 666);
    CASE(" 2-1 + 2", 3);
    CASE(" 2- (1 + 2)", -1);
    CASE("(1+(4+5+2)-3)+(6+8)", 23);
    CASE("(1+(4+5+2)-3)+6+8", 23);
    return 0;
}
