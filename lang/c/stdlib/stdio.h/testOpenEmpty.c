#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    FILE *fp = fopen("", "r");
    if (fp == NULL) {
        // If open the file "" (Its name is empty string), 
        printf("Yes, there is no '' file, err=%d:%s\n", errno, strerror(errno));
        return 0;
    }
    printf("Oops, where is '' file?\n");
    fclose(fp);
    return 0;
}
