#ifndef _KR_H_
#define _KR_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#define COLORFUL
#ifdef COLORFUL
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0;m"
#else
#define RED
#define GREEN
#define RESET
#endif
static inline const char *expect(bool v) {
    return v ? (GREEN "pass"RESET) : (RED "FAIL"RESET);
}

static inline void reverse(char *s) {
    int len = strlen(s);
    int half = len / 2;
    int i;
    char t;
    for (i = 0; i != half; i++) {
        t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}

static inline int sign(int x) {
    return ((x > 0) - (x < 0));
}

static inline bool eqDouble(double x, double y) {
    return fabs(2 * (x - y) / (x + y)) < 0.001;
}

#endif
