#include "stdio.h"
int main(void)
{
    char comment[] =
        "1st and 2nd case: array of const pointer\n"
        "\tthe string store in static section, not stack\n"
        "\tand same literal string will share storage\n"
        "\t2nd case could add last item as NULL pointer\n"
        "\tstr1 -> [ptr0, ptr1]\n"
        "\t\tptr0 -> \"012\" (static section)\n"
        "\t\tptr1 -> \"012456\" (static section)\n"
        "\n"
        "2nd case: check NULL pointer as GUARD\n"
        "\tthis will save var i, and fast(but not)\n"
        "\n"
        "3rd & 4th case:\n"
        "Cons: 1. have to hardcode max length of strings\n"
        "      2. save in stack, and same literal string NOT share\n"
        "      3. CANNOT save padding space\n"
        "\tstr3 -> {\n"
        "\t\t{0, 1, 2, \\0, x,  x, x, x, x, x}\n"
        "\t\t{0, 1, 2, 3, 4,   5, 5, \\0, x, x}\n"
        "\t\t}"
        "\n"
        "4th case:\n"
        "\tshow how to using pointer of fix length of string\n"
        "----\n"
        "so best pratice *1st case*\n"
        ;
    puts(comment);

    printf("1st case\n");
    const char* str1[] = {"012", "0123456"};
    size_t i;
    for (i = 0; i < sizeof(str1) / sizeof(str1[0]); ++i) {
        printf("%p %p %p %s\n", str1, str1 + i, str1[i], str1[i]);
    }

    printf("2nd case\n");
    const char* str2[] = {"012", "012456", NULL};
    const char **p = str2;
    for (; *p; p++) {
        printf("%p %p %p %s\n", str2, p, *p, *p);
    }

    printf("3rd case\n");
    const char str3[][10] = {"012", "0123456"};
    for (i = 0; i < sizeof(str3) / sizeof(str3[0]); i++) {
        printf("%p %p %s\n", str3, str3[i], str3[i]);
    }

    printf("4th case\n");
    char str4[][10] = {"012", "0123456"};
    char (*p4)[10] = str4;
    for (i = 0; i < sizeof(str4) / sizeof(str4[0]); i++, p4++) {
        printf("%p %p %s\n", str4, p4, *p4);
    }
    return 0;
}
