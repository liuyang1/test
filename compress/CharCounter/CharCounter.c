#include <stdio.h>

int main(int argc, char **argv) {
#define TBL_SIZE    256
    unsigned int tbl[TBL_SIZE] = {0};

    FILE *fp;
    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "ro");
    }
    while (1) {
        int c = fgetc(fp);
        if (c == EOF) {
            break;
        }
        tbl[c]++;
    }
    fclose(fp);

    int sum;
    int i;
    printf("[");
    for (i = 0, sum = 0; i != TBL_SIZE; i++) {
        if (tbl[i] != 0) {
            // printf("%02x: %d\n", i, tbl[i]);
            printf("%d, ", tbl[i]);
            sum += tbl[i];
        }
    }
    printf("]\n");
    // printf("sum: %d\n", sum);
    return 0;
}
