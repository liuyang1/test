#include <stdio.h>

int main(int argc, char *argv[])
{
    char str[1024] = "abc";
    // undefine behaviour
    sprintf(str, "%s%s", "def", str);
    printf("%s\n", str);
    return 0;
}
