#include <stdio.h>

char a[4][7] = {"Common", "Point", "Boost", "Better"};

char (*b[4])[7] = {a + 3, a + 1, a, a + 2};

char (*(*c(void))[4])[7] {
    return &b;
}

char (**d(void))[7] {
    return c()[1] - 3;
}

char buf[256];

char *pointer_monster(char (**f(void))[7])
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
