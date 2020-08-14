#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("%s filename [...]\n", argv[0]);
        printf("accumulate content of file as int32_t\n");
        return 0;
    }
    int i;
    for (i = 1; i != argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            continue;
        }
        int32_t x = 0, a;
        while (fread(&a, sizeof(int32_t), 1, fp)) {
            x += a;
        }
        fclose(fp);
        printf("%s 0x%08x\n", argv[i], x);
    }
    return 0;

}
