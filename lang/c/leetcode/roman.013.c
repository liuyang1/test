#include "leet.h"
int rchar2num(char c) {
    switch (c) {
        case 'i': return 1;
        case 'v': return 5;
        case 'x': return 10;
        case 'l': return 50;
        case 'c': return 100;
        case 'd': return 500;
        case 'm': return 1000;
        default: assert(0);
            return -1;
    }
}

int romanToInt(char *s) {
    s = strdup(s);
    char *p;
    for (p = s; *p != '\0'; p++) {
        *p = tolower(*p);
    }
    p--;
    int l = 0, x = 0, c = 0;
    for (; p >= s; p--) {
        c = rchar2num(*p);
        if (c >= l) {
            x += c;
        } else {
            x -= c;
        }
        l = c;
    }
    free(s);
    return x;
}

bool unit(char *s, int e) {
    int r = romanToInt(s);
    bool ret = r == e;
    if (!ret) {
        printf("romanToInt(%s) = %d ?= %d %s\n",
               s, r, e, expect(ret));
    }
    return ret;
}

int main() {
    unit("i", 1);
    unit("ii", 2);
    unit("iii", 3);
    unit("iv", 4);
    unit("v", 5);
    unit("vi", 6);
    unit("vii", 7);
    unit("viii", 8);
    unit("ix", 9);
    unit("x", 10);
    unit("l", 50);
    unit("c", 100);
    unit("d", 500);
    unit("m", 1000);
    unit("xl", 40);
    unit("xc", 90);
    unit("cd", 400);
    unit("cm", 900);
    unit("mcmliv", 1954);
    unit("mcmxc", 1990);
    unit("mmxiv", 2014);
    unit("dcxxi", 621);
    unit("DCXXI", 621);
    return 0;
}
