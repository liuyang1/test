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

bool isNumber_fn(char *s) {
    s = dropWhile(s, isSpace);
    s = isNum(s);
    if (s != NULL && *s == 'e') {
        s++;
        s = isExp(s);
    }
    s = dropWhile(s, isSpace);
    return s != NULL && *s == '\0';
}

// DFA style solution
typedef enum {
    eHeadBlank,
    eMainSign,
    eZhengDigit,
    eMainDot,
    eFracDot, // without maindigit, directly find one dot like .1
    eFracDigit,
    eExpSt,
    eExpSign,
    eExpDigit,
    eTailBlank,
    eFail,
} State;

typedef enum {
    eDigit,
    eBlank,
    eSign,
    eDot,
    eExp,
    eActionMax,
} Action;

static State DFA[][eActionMax] = {
    [eHeadBlank]  = {[eDigit] = eZhengDigit, [eBlank] = eHeadBlank, [eSign] = eMainSign, [eDot] = eFracDot, [eExp] = eFail},
    [eMainSign]   = {[eDigit] = eZhengDigit, [eBlank] = eFail,      [eSign] = eFail,     [eDot] = eFracDot, [eExp] = eFail},
    [eZhengDigit] = {[eDigit] = eZhengDigit, [eBlank] = eTailBlank, [eSign] = eFail,     [eDot] = eMainDot, [eExp] = eExpSt},
    [eMainDot]    = {[eDigit] = eFracDigit,  [eBlank] = eTailBlank, [eSign] = eFail,     [eDot] = eFail,    [eExp] = eExpSt},
    [eFracDot]    = {[eDigit] = eFracDigit,  [eBlank] = eTailBlank, [eSign] = eFail,     [eDot] = eFail,    [eExp] = eExpSt},
    [eFracDigit]  = {[eDigit] = eFracDigit,  [eBlank] = eTailBlank, [eSign] = eFail,     [eDot] = eFail,    [eExp] = eExpSt},
    [eExpSt]      = {[eDigit] = eExpDigit,   [eBlank] = eFail,      [eSign] = eExpSign,  [eDot] = eFail,    [eExp] = eFail},
    [eExpSign]    = {[eDigit] = eExpDigit,   [eBlank] = eFail,      [eSign] = eFail,     [eDot] = eFail,    [eExp] = eFail},
    [eExpDigit]   = {[eDigit] = eExpDigit,   [eBlank] = eTailBlank, [eSign] = eFail,     [eDot] = eFail,    [eExp] = eFail},
    [eTailBlank]  = {[eDigit] = eFail,       [eBlank] = eTailBlank, [eSign] = eFail,     [eDot] = eFail,    [eExp] = eFail},
    [eFail]       = {eFail},
};

Action char2act(char c) {
    if (c >= '0' && c <= '9') {
        return eDigit;
    }
    switch(c) {
        case ' ': return eBlank;
        case '+':
        case '-': return eSign;
        case '.': return eDot;
        case 'e': return eExp;
        default: return eActionMax;
    }
}

bool isNumber_dfa(char *s) {
    State st = eHeadBlank;
    for (; *s != '\0' && st != eFail; s++) {
        Action act = char2act(*s);
        if (act == eActionMax) {
            st = eFail;
            break;
        }
        st = DFA[st][act];
    }
    return st == eZhengDigit || st == eFracDigit || st == eExpDigit ||
          st == eTailBlank || st == eMainDot;
}

#define WEAK_ALIAS(name, aliasname) \
    extern typeof(name) aliasname __attribute((weak, alias(# name)));

// WEAK_ALIAS(isNumber_fn, isNumber);
WEAK_ALIAS(isNumber_dfa, isNumber);

#define CASE(s, e) {bool r = isNumber(s);         \
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
