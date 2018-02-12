#include <stdio.h>

int main(void)
{
    printf("EOF: %d\n", EOF);
    printf("%d\n", fputc('a', stdin));
    printf("return EOF, when try to write to stdin as it's READ-ONLY stream\n");
    return 0;
}
