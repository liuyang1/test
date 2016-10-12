#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "font.xbm"

#define DATA font_bits
int test()
{
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    unsigned int row = 0;
    unsigned char c;
    for (j = 0; j < font_width; j ++) {
        printf("%d", j % 10);
    }
    printf("\n");
    for (i = 0; i < sizeof(DATA) / sizeof(char); i++) {
        c = DATA[i];
        for (j = 0; j < 8; j++) {
            if (c & (1 << j)) {
                putchar('@');
            } else {
                putchar(' ');
            }
            k++;
            if ((k + 1) % font_width == 0) {
                printf("|%d\n", row);
                k = 0;
                row++;
                break;
            }
        }
    }
    return 0;
}

int char2offset(char c)
{
    return (c - ' ') * (font_width_per_char + 1);
}

int drawTextOneLine(char *text)
{
    size_t len = strlen(text);
    int *offsetTbl = (int *)malloc(sizeof(int) * len);
    size_t i, j, k, u;
    for (i = 0; i < len; i++) {
        offsetTbl[i] = char2offset(text[i]);
        printf("map text[%lu]='%c' to offset=%d\n", i, text[i], offsetTbl[i]);
    }
    char *prow = font_bits;
    char *pc;
    for (j = 0; j < font_height; j++) {
        for (i = 0; i < len; i++) {
            pc = prow + offsetTbl[i] / 8;
            u = offsetTbl[i] % 8;
            for (k = 0; k < font_width_per_char + 2; k++) {
                if ((*pc) & (1 << u)) {
                    putchar('@');
                } else {
                    putchar(' ');
                }
                u++;
                if (u == 8) {
                    pc += 1;
                    u = 0;
                }
            }
            putchar('|');
        }
        prow += font_width / 8;
        if (font_width % 8 != 0) {
            prow++;
        }
        printf("|%lu line\n", j);
    }
    free(offsetTbl);
    return 0;
}



int main() {
    return drawTextOneLine(" !01234");
}
