int titleToNumber(char* s) {
    int r, i;
    for (r = 0; *s != '\0'; s++) {
        r = r * 26 + (*s - 'A' + 1);
    }
    return r;
}

#define CASE(s, e) {int x = titleToNumber(s); printf("%s %d ?= %d %s\n", \
                                                     s, x, e, x == e ? "PASS" : "fail"); }
int main()
{
    CASE("", 0);
    CASE("A", 1);
    CASE("AA", 27);
    CASE("ZZ", 702);
    CASE("AAA", 703);
    CASE("ABC", 731);
    CASE("XYZ", 16900);
    return 0;
}
