#include <stdio.h>
#include <stdbool.h>
bool checkRecord(char *s) {
    size_t a, mcl, cl;
    for (a = cl = mcl = 0; *s != '\0'; s++) {
        a += *s == 'A';
        if (*s != 'L') {
            cl = 0;
        } else {
            cl++;
            if (mcl < cl) {
                mcl = cl;
            }
        }
    }
    // printf("a=%zu cl=%zu mcl=%zu\n", a, cl, mcl);
    return !(a > 1 || mcl > 2);
}

char *show_bool(bool b) { return b ? "true" : "false"; }
char *show_ret(bool b) { return b ? "pass" : "FAIL"; }
int unit(char *s, bool e) {
    bool r = checkRecord(s);
    printf("%s expect=%s return=%s %s\n", s, show_bool(e), show_bool(r),
           show_ret(e == r));
    return 0;
}

int main() {
    unit("PPALLP", true);
    unit("LALL", true);
    unit("PPALLL", false);
    return 0;
}
