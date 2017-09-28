char *addBinary(char *a, char *b) {
    int alen = strlen(a);
    int blen = strlen(b);
    int rlen = (alen > blen ? alen : blen) + 1;
    char *r = malloc(sizeof(char) * rlen + 1);
    r[rlen] = '\0';

    int i, j, k, carry;
    for (i = alen - 1, j = blen - 1, k = rlen - 1, carry = 0;
         i >= 0 || j >= 0;
         i--, j--, k--, carry /= 2) {
        if (i >= 0) {
            carry += a[i] - '0';
        }
        if (j >= 0) {
            carry += b[j] - '0';
        }
        r[k] = carry % 2 + '0';
    }
    r[0] = carry + '0';

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
    CASE("0001", "1000", "1001");
    CASE(
        "110010100000000000010111100111100101010010111101101100010001111101100110101011000110111111110000",
        "11000000001101110101000111110011100100100001110111011010001001",
        "110010100000000000010111100111101000010011001011100001011001110001001011001100111110011001111001");

    return 0;
}
