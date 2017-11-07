#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

unsigned int cube(unsigned int n) {
    return n * n * n;
}

bool isCubes(unsigned int n) {
    unsigned int s, d;
    for (s = 0, d = n; d != 0; d /= 10) {
        s += cube(d % 10);
    }
    return s == n;
}

char *isSumOfCubes(char *p) {
    size_t offset = 0, len = (strlen(p) / 3 + 1) * 4 +
                             1 + 10 + strlen("Unlucky") + 1;
    char *out = malloc(sizeof(char) * len);
#define PUT(...) offset += snprintf(out + offset, len - 1 - offset, __VA_ARGS__);
    unsigned int cubecnt = 0, sum = 0;
    for (unsigned int num = 0, cnt = 0;; p++) {
        if (isdigit(*p)) {
            num = num * 10 + *p - '0';
            cnt++;
        }
        if (cnt == 3 || !isdigit(*p)) {
            if (cnt != 0 && isCubes(num)) {
                PUT("%s%u", cubecnt == 0 ? "" : " ", num);
                sum += num;
                cubecnt++;
            }
            num = cnt = 0;
        }
        if (*p == '\0') {
            break;
        }
    }
    if (cubecnt == 0) {
        PUT("%s", "Unlucky");
    } else {
        PUT(" %u %s", sum, "Lucky");
    }
    assert(offset + 1 < len);
    return out;
}

#include "test.h"
bool unit_cube(int n, bool expect) {
    bool ret = isCubes(n);
    bool r = ret == expect;
    if (!r) {
        printf("cube(%d) = %s !!!= %s\n",
               n, showBool(ret), showBool(expect));
    }
    return r;
}

int test_cube() {
    unit_cube(0, true);
    unit_cube(1, true);
    unit_cube(407, true);
    unit_cube(153, true);
    return 0;
}

bool unit_isSumOfCubes(char *str) {
    char *ret = isSumOfCubes(str);
    printf("isSumOfCubes(%s) = %s\n", str, ret);
    free(ret);
    return true;
}

int test_isSumOfCubes() {
    unit_isSumOfCubes("");
    unit_isSumOfCubes("aqdf& 0 1 xyz 153 777.777");
    unit_isSumOfCubes("24172410");
    unit_isSumOfCubes("QK29 45[&erui");
    unit_isSumOfCubes("001234");
    unit_isSumOfCubes("0785");
    unit_isSumOfCubes("153153153153153153153");
    return 0;
}

int main() {
    test_cube();
    test_isSumOfCubes();
    return 0;
}
