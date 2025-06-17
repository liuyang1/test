#include <ctype.h>
#include <string.h>
#include <stdio.h>

void show_hex(char *s, size_t len) {
    size_t i;
    for (i = 0; i != len; i++) {
        printf("%02x%c ", s[i], isprint(s[i]) ? s[i] : ' ');
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    if (i % 8 != 0) {
        printf("\n");
    }
}

#if 0
int test_strcpy() {
    char str[] = "01234567"; // it's memory is 8+1=9char
    size_t len = sizeof(str);
    printf("sizeof(str)=%lu\n", len);
    strcpy(str, "hello, world!\n"); // it's overflow
    printf("str=%s\n", str);
    strcpy(str, "test");
    printf("str=%s\n", str);
    show_hex(str, 32);

    strcpy(str, "0123456789abcdef");
    strncpy(str, "hello, world!\n", len);
    printf("str=%s\n", str);
    show_hex(str, 32);

    return 0;
}
#endif

int test_strncpy() {
    char str[] = "01234567";
    size_t len = sizeof(str);
    printf("str=%s\n", str);

    strncpy(str, "hello, world\n", len);
    /**
     * This is wrong, strncpy just ensure copy as most n bytes to dest buffer
     * However, it doesn't consider '\0' or nil char
     * So it's dangerous when directly printf to show it
     * One workaroud solution, is fill '\0' to last char
     */
    // printf("str=%s\n", str);
    int width = len - 1;
    printf("str=%*s\n", width, str);
    if (len > 0) {
        str[len - 1] = '\0';
    }

    printf("str=%s\n", str);

    strncpy(str, "hello", len);
    if (len > 0) {
        str[len - 1] = '\0';
    }

    printf("str=%s\n", str);
    return 0;
}

int unit_strncoy_logic(char *src) {
#define LEN 8
    char dst[] = "01234567";

    size_t src_len = strlen(src);
    if (src_len >= LEN) {
        printf("over size skip src=%s\n", src);
        return -1;
    }
    strncpy(dst, src, LEN);
    if (strcmp(src, dst) == 0) {
        printf("SUCC dst=%s src=%s\n", dst, src);
        retur/Users/lyz/git/amz/00.valut/01.wiki/amzn/business.wiki
            0;
    } else {
        printf("FAIL dst=%s src=%s\n", dst, src);
        return -1;
    }
}

int test_strncpy_logic() {
    unit_strncoy_logic("abc");
    unit_strncoy_logic("abcdef");
    unit_strncoy_logic("abcdefg");
    unit_strncoy_logic("abcdefgh");
    unit_strncoy_logic("abcdefghi");
    return 0;
}

int main() {
    // test_strncpy();
    test_strncpy_logic();

    return 0;
}
