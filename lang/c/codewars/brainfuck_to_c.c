/* ref: https://www.codewars.com/kata/brainfuck-translator/train/c
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// #define LOG printf
#define LOG(...)
const char validCharTbl[] = "+-<>,.[]";

bool any(char c, const char *p) {
    bool r = false;
    for (int k = 0; p[k] != '\0'; k++) {
        r = r || c == p[k];
    }
    return r;
}

void omit(char *in) {
    int i, j;
    for (i = j = 0; in[i] != '\0'; i++) {
        if (any(in[i], validCharTbl)) {
            in[j++] = in[i];
        }
    }
    in[j] = '\0';
}

bool pair(char a, char b, char x, char y) {
    return (a == x && b == y) || (a == y && b == x);
}

// no change: return false; change, reutrn true;
bool reduceOne(char *in) {
    bool ret = false;
    if (strlen(in) <= 1) {
        return ret;
    }
    char *out = in;
    int i, j = 0;
    for (i = 0; in[i] != '\0' && in[i + 1] != '\0'; i++) {
        if ((pair(in[i], in[i + 1], '+', '-')) ||
            (pair(in[i], in[i + 1], '<', '>')) ||
            (in[i] == '[' && in[i + 1] == ']')) {
            i++;
            ret = true;
        } else {
            out[j++] = in[i];
        }
    }
    if (in[i]) {
        out[j++] = in[i]; // save last char
    }
    out[j] = '\0';
    return ret;
}

char *reduce(char *in) {
    while (reduceOne(in)) {
    }
    return in;
}

char *opt(const char *in) {
    char *s = strdup(in);
    omit(s);
    LOG("after omit: %s\n", s);
    char *out = reduce(s);
    return out;
}

bool validBracket(const char *in) {
    int i, cnt;
    for (i = cnt = 0; in[i] != '\0'; i++) {
        cnt += in[i] == '[';
        cnt -= in[i] == ']';
        if (cnt < 0) {
            return false;
        }
    }
    if (cnt > 0) {
        return false;
    }
    return true;
}

#define PUT(...)  {                                                     \
        for (int i = 0; i != shift; i ++) {                             \
            offset += snprintf(out + offset, OUTLEN - offset, " ");     \
        }                                                               \
        offset += snprintf(out + offset, OUTLEN - offset, __VA_ARGS__); \
        offset += snprintf(out + offset, OUTLEN - offset, "\n");        \
        if (offset + 64 > OUTLEN) {                                     \
            OUTLEN *= 2;                                                \
            LOG("buffer=%lx %ld\n", OUTLEN, OUTLEN);                    \
            char *new = realloc(out, OUTLEN);                           \
            if (new == NULL) {                                          \
                printf("cannot reallocated buffer\n");                  \
                free(out);                                              \
                assert(0);                                              \
            }                                                           \
            out = new;                                                  \
        }                                                               \
}

char *translate(char *in) {
    size_t OUTLEN = 128;
    char *out = malloc(sizeof(char) * OUTLEN);
    out[0] = '\0';
    size_t offset = 0;
    int i, last, cnt, shift;
    for (i = last = cnt = shift = 0;; i++) {
        if (last != 0 && in[i] != last) {
            switch (last) {
                case '+': PUT("*p += %d;", cnt); break;
                case '-': PUT("*p -= %d;", cnt); break;
                case '>': PUT("p += %d;", cnt); break;
                case '<': PUT("p -= %d;", cnt); break;
                default:
                    assert(0);
            }
            last = cnt = 0;
        }
        if (in[i] == '\0') {
            break;
        }
        switch (in[i]) {
            case '+':
            case '-':
            case '>':
            case '<':
                last = in[i];
                cnt++;
                break;
            case '.': PUT("putchar(*p);"); break;
            case ',': PUT("*p = getchar();"); break;
            case '[': PUT("if (*p) do {"); shift += 2; break;
            case ']': shift -= 2; PUT("} while (*p);"); break;
            default:
                assert(0);
        }
    }
    return out;
}

char *brainfuck_to_c(const char *source) {
    if (!validBracket(source)) {
        return strdup("Error!");
    }
    char *opted = opt(source);
    char *out = translate(opted);
    free(opted);
    return out;
}

bool unit_opt(char *in, char *expect) {
    char *ret = opt(in);
    bool r = strcmp(ret, expect) == 0;
    if (!r) {
        printf("opt(%s) = %s %s %s\n",
               in, ret, r ? "==" : "!!!=", expect);
    }
    free(ret);
    return r;
}

int test_opt() {
    unit_opt("+.-.<.>[.].,", "+.-.<.>[.].,");
    unit_opt("+.-.aasdf<.>a{a}[.].,;", "+.-.<.>[.].,");
    unit_opt("+-", "");
    unit_opt(".", ".");
    unit_opt("+-asdf<>{}[].,;", ".,");
    unit_opt("++--+.", "+.");
    unit_opt("[][++]", "[++]");
    unit_opt("<>><", "");
    return 0;
}

int unit_valid(const char *in, bool expect) {
    bool ret = validBracket(in);
    bool r = ret == expect;
    if (!r) {
        printf("valid(%s) = %s %s %s\n",
               in, ret ? "true" : "false",
               r ? "==" : "!!!=",
               expect ? "true" : "false");
    }
    return r;
}

int test_valid() {
    unit_valid("[][][]", true);
    unit_valid("[[[]]]", true);
    unit_valid("[[[]]", false);
    unit_valid("[[]]]", false);
    return 0;
}

bool unit_translate(const char *in, const char *expect) {
    char *ret = brainfuck_to_c(in);
    bool r = strcmp(ret, expect) == 0;
    if (!r) {
        printf("brainfunc_to_c(%s) = %s %s %s\n",
               in, ret, r ? "==" : "!!!=", expect);
    }
    free(ret);
    return r;
}

int test_translate() {
    // unit_translate("+++++[>++++.<-]", "");
    unit_translate("++++", "*p += 4;\n");
    unit_translate("----", "*p -= 4;\n");
    unit_translate(">>>>", "p += 4;\n");
    unit_translate("<<<<", "p -= 4;\n");
    unit_translate(".", "putchar(*p);\n");
    unit_translate(",", "*p = getchar();\n");
    unit_translate("[[[]]", "Error!");
    unit_translate("[.]", "if (*p) do {\n  putchar(*p);\n} while (*p);\n");
    unit_translate("[][]", "");
    return 0;
}

int test_memory() {
    int len = 4096 * 100;
    char *s = malloc(sizeof(char) * (len + 1));
    int i;
    for (i = 0; i != len; i++) {
        s[i] = '.';
    }
    s[i] = '\0';
    char *ret = brainfuck_to_c(s);
    printf("result: %ld\n", strlen(ret));
    free(ret);
    free(s);
    return 0;
}

int main() {
    test_opt();
    test_valid();
    test_translate();
    test_memory();
    return 0;
}
