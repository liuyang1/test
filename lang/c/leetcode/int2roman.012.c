#include "leet.h"
char num2rchar(int x) {
    switch (x) {
        case 1: return 'I';
        case 5: return 'V';
        case 10: return 'X';
        case 50: return 'L';
        case 100: return 'C';
        case 500: return 'D';
        case 1000: return 'M';
        default:
            assert(0);
            return '0';
    }
}

char *num2rchars(int x, int base, char *s) {
#define PUT(v) *--s = num2rchar(v * base);
    switch (x) {
        case 9:
            PUT(10);
            PUT(1);
            break;
        case 8:
            PUT(1);
        case 7:
            PUT(1);
        case 6:
            PUT(1);
        case 5:
            PUT(5);
            break;
        case 4:
            PUT(5);
            PUT(1);
            break;
        case 3:
            PUT(1);
        case 2:
            PUT(1);
        case 1:
            PUT(1);
            break;
    }
    return s;
}

// max to 3999, so length <= 3 * 4 = 12
#define RLEN (3 * 4 + 1)

char *intToRoman(int num) {
    char *o, *r;
    o = r = malloc(sizeof(*r) * RLEN);
    char *p = r + RLEN - 1;
    *p = '\0';
    int j;
    for (j = 1; p != r - 1 && num != 0; j *= 10) {
        int x = num % 10;
        p = num2rchars(x, j, p);
        num = num / 10;
    }
    if (p != o) {
        memmove(o, p, strlen(p) + 1);
    }
    return o;
}

bool unit(int num, char *e) {
    char *r = intToRoman(num);
    bool ret = strcmp(r, e) == 0;
    if (!ret) {
        printf("intToRoman(%d) = %s ?= %s %s\n",
               num, r, e, expect(ret));
    }
    free(r);
    return ret;
}

int main() {
    unit(1, "I");
    unit(2, "II");
    unit(3, "III");
    unit(4, "IV");
    unit(5, "V");
    unit(6, "VI");
    unit(7, "VII");
    unit(8, "VIII");
    unit(9, "IX");
    unit(10, "X");
    unit(50, "L");
    unit(100, "C");
    unit(500, "D");
    unit(1000, "M");
    unit(40, "XL");
    unit(90, "XC");
    unit(400, "CD");
    unit(900, "CM");
    unit(1954, "MCMLIV");
    unit(1990, "MCMXC");
    unit(2014, "MMXIV");
    unit(621, "DCXXI");
}
