#include <stdio.h>

int shell() {
    execvp("/bin/sh", NULL);
    return 0;
}
int main() {
    int flag = 0;
    char input[1] = {0};
    printf("input:%p flag=%p\n", input, &flag);
    printf("main=%p shell=%p\n", main, shell);
    scanf("%s", input);
    if (flag != 0) {
        printf("overflow flag=%x\n", flag);
        return -1;
    }
    return 0;
}
