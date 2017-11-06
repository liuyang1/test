/* demo: how to use strrchr function
 */
#include <string.h>
#include <stdio.h>

int main() {
    char str[] = "E_liuyang.mp4";
    char *p = NULL;
    printf("%s\n", str);
    p = strrchr(str, '.');
    if (p != NULL) {
        *p = '\0';
        printf("find .\n");
    } else {
        printf("not find .\n");
    }
    printf("%s\n", str);
    p = strrchr(str, '_');
    if (p != NULL) {
        printf("find _\n");
        *p = '\0';
    }
    printf("%s\n", str);
    printf("%s\n", p + 1);
    return 0;
}
