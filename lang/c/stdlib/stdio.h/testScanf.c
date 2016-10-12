#include <stdio.h>
void testWhitespaceStr() {
    char str[128];
    scanf("%[^\n]", str);
    printf("%s\n", str);
}

int main() {
    testWhitespaceStr();
    return 0;
}
