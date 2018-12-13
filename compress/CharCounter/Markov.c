#include <stdio.h>

int main(int argc, char **argv) {
#define TBL_SIZE    256
    unsigned int tbl[TBL_SIZE][TBL_SIZE] = {{0}};

    FILE *fp;
    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "ro");
    }
    int lastc = -1;
    while (1) {
        int c = fgetc(fp);
        if (c == EOF) {
            break;
        }
        if (lastc != -1) {
            tbl[lastc][c]++;
        }
        lastc = c;
    }
    fclose(fp);

    int i, j;
    printf("[");
    for (j = 0; j != TBL_SIZE; j++) {
        if (j != 0) {
            printf(", ");
        }
        printf("[");
        for (i = 0; i != TBL_SIZE; i++) {
            if (i != 0) {
                printf(", ");
            }
            // printf("%02x: %d\n", i, tbl[i]);
            printf("%d", tbl[j][i]);
        }
        printf("]\n");
    }
    printf("]\n");
    return 0;
}
