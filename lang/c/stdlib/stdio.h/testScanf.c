#include <stdio.h>
void testWhitespaceStr() {
    char str[128];
    /*  It matches a string of all characters
     *      not equal to the new line character ('\n')
     *  and stores it (plus a terminating '\0' character) in str
     */
    scanf("%[^\n]", str);
    printf("%s\n", str);
}

void testOutputPointer() {
    void *p = (void *)0x100;
    printf("print pointer: %p\n", p);
}

void testInputPointer(char *s) {
    void *pp = NULL;
    int r = sscanf(s, "%p", &pp);
    printf("print pointer: r=%d s='%s' %p (at %p)\n",
           r, s, pp, &pp);
}

void testPointer() {
    testOutputPointer();
    testInputPointer("0x200");
    testInputPointer("0X200");
    testInputPointer("200");
    testInputPointer("2aa");
    testInputPointer("xxx");
}

int main() {
    testWhitespaceStr();
    testPointer();
    return 0;
}
