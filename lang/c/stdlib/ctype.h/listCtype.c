#include <stdio.h>
#include <ctype.h>

static const char *show_bool(int i) {
    return i ? "T" : "F";
}

int main() {
    int i;
    for (i = 0; i != 0x80; i++) {
        printf("%3d 0x%02x '%c' ascii=%s blank=%s cntrl=%s "
               "graph=%s print=%s punct=%s space=%s xdigit=%s alnum=%s\n",
               i, i, isprint(i) ? i : '.',
               show_bool(isascii(i)),
               show_bool(isblank(i)),
               show_bool(iscntrl(i)),
               show_bool(isgraph(i)),
               show_bool(isprint(i)),
               show_bool(ispunct(i)),
               show_bool(isspace(i)),
               show_bool(isxdigit(i)),
               show_bool(isalnum(i)));
    }
    return 0;
}
