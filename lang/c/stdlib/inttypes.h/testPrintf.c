#include <stdio.h>
#include <inttypes.h>

#define SHOW(x)     printf("%s:%s\n", # x, x);
void showFormat() {
    SHOW(PRId8);
    SHOW(PRId16);
    SHOW(PRId32);
    SHOW(PRId64);
    SHOW(PRIi8);
    SHOW(PRIi16);
    SHOW(PRIi32);
    SHOW(PRIi64);
    SHOW(PRIo8);
    SHOW(PRIo16);
    SHOW(PRIo32);
    SHOW(PRIo64);
    SHOW(PRIu8);
    SHOW(PRIu16);
    SHOW(PRIu32);
    SHOW(PRIu64);
    SHOW(PRIx8);
    SHOW(PRIx16);
    SHOW(PRIx32);
    SHOW(PRIx64);
    SHOW(PRIX8);
    SHOW(PRIX16);
    SHOW(PRIX32);
    SHOW(PRIX64);
}

int main() {
    showFormat();
    uint32_t a = 0;
    printf("a=%d\n", a);
    puts("a=%" PRIu32 "\n");
    printf("a=%" PRIu32 "\n", a);
    return 0;
}
