#include <stdio.h>
#include <string.h>

int main()
{
    char s[1024];

    fgets(s, 5, stdin);
    printf("%s\n", s);

    getchar();
    fgets(s, -1, stdin);
    printf("%s\n", s);

    getchar();
    fgets(s, 0, stdin);
    printf("%s\n", s);
    return 0;
}
