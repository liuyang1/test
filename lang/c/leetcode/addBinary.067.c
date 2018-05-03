/**
 * addBinary on bit string (no leading zero)
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int char2digit(char c) {
    return c - '0';
}

char digit2char(int c) {
    return c + '0';
}

char *addBinary(char *a, char *b) {
    int alen = strlen(a);
    int blen = strlen(b);
    int rlen = (alen > blen ? alen : blen) + 1;
    char *r = malloc(sizeof(char) * rlen + 1);
    r[rlen] = '\0';

    int i, j, k, s, carry;
    for (i = alen - 1, j = blen - 1, k = rlen - 1, carry = 0;
         i >= 0 || j >= 0;
         i--, j--, k--) {
        s = (i >= 0) * char2digit(a[i]) + (j >= 0) * char2digit(b[j]) + carry;
        r[k] = digit2char(s % 2);
        carry = s / 2;
    }
    r[0] = digit2char(carry);

    // remove extra leading zero, but keep it if only one zero
    for (i = 0; i != rlen; i++) {
        if (r[i] != '0') {
            break;
        }
    }
    if (i != 0) {
        if (i == rlen) {
            i = rlen - 1;
        }
        for (j = i; j != rlen + 1; j++) {
            r[j - i] = r[j];
        }
    }
    return r;
}

#define CASE(a, b, e) {char *r = addBinary(a, b);                               \
                       printf("%s + %s = %s ?= %s %s\n",                        \
                              a, b, r, e, strcmp(r, e) == 0 ? "PASS" : "fail"); \
                       free(r); }
int main()
{
    CASE("0", "0", "0");
    CASE("1", "1000", "1001");
    CASE("1111", "1", "10000");
    CASE("1111", "10001", "100000");
    CASE(
        "110010100000000000010111100111100101010010111101101100010001111101100110101011000110111111110000",
        "11000000001101110101000111110011100100100001110111011010001001",
        "110010100000000000010111100111101000010011001011100001011001110001001011001100111110011001111001");

    return 0;
}
