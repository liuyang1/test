#include <stdio.h>

void usage(char *name) {
    printf("usage: %s val...\n", name);
    printf("show value with dec, hex, bin format\n");
}
void show_delimter(size_t j) {
    if (j % 8 == 0) {
        printf(" ");
    } else if (j % 4 == 0) {
        printf("-");
    }
}
#define MAX_BIT         128
#define CHUNK           8
void show_bin(unsigned int n) {
    // decimal
    printf("%u\n", n);

    unsigned int buf[MAX_BIT] = {0};
    size_t i = 0, j;
    for (i = 0; i != MAX_BIT; i++) {
        buf[i] = n % 2;
        n /= 2;
        if (n == 0) {
            break;
        }
    }
    i++;
    i = ((i + CHUNK - 1) / CHUNK) * CHUNK; // round up to mutiples of CHUNK

    // hex
    for (j = i; j != 0; j -= 4) {
        printf("   %x",
               (buf[j - 1] << 3) | (buf[j - 2] << 2) | (buf[j - 3] << 1) | buf[j - 4]);
        show_delimter(j - 4);
    }
    printf("\n");

    // binary
    for (j = i; j != 0; j--) {
        printf("%d", buf[j - 1]);
        show_delimter(j - 1);
    }
    printf("\n");
    // ruler with decimal
    for (j = i; j != 0; j--) {
        printf("%lu", (j - 1) % 10);
        show_delimter(j - 1);
    }
    printf("\n");
    if (i <= 10) {
        return;
    }
    // ruler with decimal
    for (j = i; j != 0; j--) {
        printf("%lu", (j - 1) / 10);
        show_delimter(j - 1);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        usage(argv[0]);
        return 0;
    }
    int i;
    for (i = 1; i != argc; i++) {
        show_bin(strtoul(argv[i], NULL, 0));
    }
    return 0;
}
