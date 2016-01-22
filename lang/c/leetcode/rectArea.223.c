#include <stdio.h>
/*
 * define interface for line(1-D) or rect(2-D)
 * # stretch
 * # overlap
 * # area
 */
typedef struct {
    int origin;
    int length;
} line;

void showLine(line a) {
    printf("%d-%d\n", a.origin, a.origin + a.length);
}

line overlap1(line a, line b) {
    if (a.origin > b.origin) {
        return overlap1(b, a);
    }
    line r;
    // 1. no overlap
    // a --------
    //                b ----
    // 2. b in a
    // a --------
    //   b ----
    // 3. a, b overlap
    // a --------
    //       b ----
    if (a.origin + a.length <= b.origin) {
        r.origin = 0;
        r.length = 0;
    } else if (a.origin + a.length >= b.origin + b.length) {
        r.origin = b.origin;
        r.length = b.length;
    } else {
        r.origin = b.origin;
        r.length = a.origin + a.length - b.origin;
    }
    return r;
}

line stretch1(int A, int B) {
    if (A > B) {
        return stretch1(B, A);
    }
    line a;
    a.origin = A;
    a.length = B - A;
    return a;
}

unsigned int area1(line a) {
    return a.length;
}

typedef struct {
    line x;
    line y;
} rect;

void showRect(rect a) {
    printf("(%d,%d)-(%d,%d)\n", a.x.origin, a.y.origin,
           a.x.origin + a.x.length, a.y.origin + a.y.length);
}

rect overlap2(rect a, rect b) {
    rect r;
    r.x = overlap1(a.x, b.x);
    r.y = overlap1(a.y, b.y);
    return r;
}

unsigned int area2(rect a) {
    return area1(a.x) * area1(a.y);
}

rect stretch2(int A, int B, int C, int D) {
    rect a;
    a.x = stretch1(A, C);
    a.y = stretch1(B, D);
    return a;
}

int computeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
    rect a = stretch2(A, B, C, D);
    rect b = stretch2(E, F, G, H);
    rect r = overlap2(a, b);
    return area2(a) + area2(b) - area2(r);
}

#define CASE2(a, b, c, d, e, f, g, h, xpct) { \
        int r = computeArea(a, b, c, d, e, f, g, h); \
        printf("%d ?= %d %s\n", r, xpct, r == xpct ? "PASS" : "fail"); \
}
int main() {
    CASE2(-3, 0, 3, 4, 0, -1, 9, 2, 45);
    CASE2(-2, -2, 2, 2, -2, -2, 2, 2, 16);
    CASE2(0, 0, 0, 0, -1, -1, 1, 1, 4);
    return 0;
}
