#include <stdio.h>

int MAX_WID = (10000);

int main() {
    printf("1\n");
    printf("%u %u\n", MAX_WID, MAX_WID);
    int i, j;
    for (i = 0; i != MAX_WID; i++) {
        for (j = 0; j != MAX_WID; j++) {
            printf(".");
        }
        printf("\n");
    }
    return 0;
}
