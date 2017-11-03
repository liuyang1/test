#include <stdio.h>

// Sorry, using "typedef" is considered cheat code.
char a[4][7] = {"Common", "Point", "Boost", "Better"};
typedef char FixedStr[7];

FixedStr *b[4] = {a + 3, a + 1, a, a + 2};
typedef FixedStr *FixedStrArr[4];

FixedStrArr *c(void) {
    return &b;

}

FixedStr **d(void) {
    return c()[1] - 3;
}

char buf[256];

typedef FixedStr **(Func)(void);

char *pointer_monster(Func f)
{
    int len;

    len  = sprintf(buf, "%s", *f()[0]); // Point
    len += sprintf(buf + len, "%s ", *((**f)() - 1)[0] + 4); // er
    len += sprintf(buf + len, "%s", (*f())[0] - 4); // mon
    len += sprintf(buf + len, "%s", f()[1][2] + 3); // st
    len += sprintf(buf + len, "%s", *((**f)() - 1)[0] + 4); // er
    return buf;
}

int main() {
    printf("%s\n", pointer_monster(d));
    return 0;
}
