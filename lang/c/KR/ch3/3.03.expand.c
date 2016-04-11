#include "../kr.h"

int expand_c(char *dst, int lim, char start, char end) {
    int i;
    char c;
    for (i = 0, c = start; i < lim && c <= end; i++, c++) {
        dst[i] = c;
    }
    return i;
}

/**
 * @brief find if match "X-Y-Z-U" pattern in src, at first colon position
 * X, Y, Z, U must increasing
 *
 * @param str
 * @param firstColon first colon postion
 *
 * @return Final colon postion, if not match, return -1
 */
int findColonPattern(const char *str, int firstColon) {
    if (firstColon == 0) {
        return -1;
    }
    char last = str[firstColon - 1];
    if (last == '-') {
        return -1;
    }
    int idx = -1;
    int i;
    for (i = firstColon; str[i] != '\0'; i += 2) {
        if (str[i] == '-' && str[i + 1] != '-' && last < str[i + 1]) {
            last = str[i + 1];
            idx = i;
        } else {
            break;
        }
    }
    return idx;
}

int expand(char *dst, int lim, const char *src) {
    if (lim == 0) {
        return 0;
    }
    char last;
    int i, j, ret;
    for (last = -1, i = j = 0; j != lim - 1;) {
        ret = findColonPattern(src, i);
        if (ret == -1) {
            // delay one char to put to DST, as findColonPattern need one delay
            if (last != -1) {
                dst[j++] = last;
            }
            last = src[i++];
            if (last == '\0') {
                break;
            }
        } else {
            j += expand_c(dst + j, lim - j, src[i - 1], src[ret + 1]);
            i = ret + 2;
            last = -1;
        }
    }
    dst[j] = '\0';
    return j;
}

#define case_colon(s, first, e) {int r = findColonPattern(s, first); \
                                 printf("%s %d %d ?= %d %s\n",       \
                                        s, first, r, e, expect(r == e)); }
void test_findColonPattern() {
    case_colon("x-z", 1, 1);
    case_colon("a-c-d", 1, 3);
    case_colon("--c-d", 1, -1);
    case_colon("a-cd-", 1, 1);
    case_colon("a-cd-e-f-g", 4, 8);
    case_colon("a", 1, -1);
}
#define STRLEN 1024
#define CASE(name, s, e) {char r[STRLEN];                        \
                          expand(r, STRLEN, s);                  \
                          printf("%-20s %-10s -> %s ?= %s %s\n", \
                                 name, s, r, e,                  \
                                 expect(strcmp(r, e) == 0)); }
int main() {
    test_findColonPattern();
    CASE("expand digit", "0-9", "0123456789");
    CASE("should not reverse", "9-0", "9-0");
    CASE("expand letter", "a-z", "abcdefghijklmnopqrstuvwxyz");
    CASE("expand upper case", "A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    CASE("muptiple 1", "A-Z0-9", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    CASE("muptiple 2", "0-9A-Z", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    CASE("continuous colon", "a-c-xx-z", "abcdefghijklmnopqrstuvwxxyz");
    CASE("literal", "abc", "abc");
    CASE("literal 1", "a--c", "a--c");
    CASE("heading colon 1", "-abc", "-abc");
    CASE("heading colon 2", "-a-d", "-abcd");
    CASE("heading colon 3", "--a-d", "--abcd");
    CASE("trailing colon 1", "-a-d-", "-abcd-");
    CASE("trailing colon 2", "-acd-", "-acd-");
    CASE("trailing colon 3", "-acd--", "-acd--");
    CASE("complex", "-aca-ed0-9-", "-acabcded0123456789-");
    return 0;
}
