#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define BIT_BYTE    8
#define _f(fmt) "%-10sL%2d "fmt"\n", __func__, __LINE__
#if 0
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

typedef struct {
    int bits;
    int offset;
    int x;
} bstrm;

void* bs_create(int bits) {
    bstrm *p = malloc(sizeof(bstrm));
    p->bits = bits;
    p->offset = 0;
    return p;
}
void bs_destroy(bstrm *p) {
    free(p);
}
const static char offset2bit[][2] = {
    {0xff, 0},
    {0x7f, 0x80},
    {0x3f, 0xc0},
    {0x1f, 0xe0},
    {0x0f, 0xf0},
    {0x07, 0xf8},
    {0x03, 0xfc},
    {0x01, 0xfe},
};
bool bs_push(bstrm *p, int in) {
    DEBUG(_f("in=0x%2x"), in);
    assert(p->offset == 0);
    p->x = in;
    return true;
}
bool bs_pop(bstrm *p, int *out) {
    int bits = p->bits;
    int mask = (1 << (bits)) - 1;
    *out = (p->x >> (p->offset)) & mask;
    p->offset += bits;
    p->offset %= 8;
    DEBUG(_f("out=0x%2x mask=0x%2x bits=%d off=%d"), *out, mask, p->bits, p->offset);
    return p->offset != 0;
}

int main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "rb");
    int bits = atoi(argv[2]);
    int syms = 1 << bits;
    int *cnt = malloc(sizeof(int) * syms);
    int c;
    void *p = bs_create(bits);
    bool b;
    while ((c = fgetc(fp)) != EOF) {
        if (bs_push(p, c)) {
            while ((b = bs_pop(p, &c)), cnt[c]++, b) {
            }
        }
    }
    int i;
    for (i = 0; i != syms; i++) {
        if (cnt[i]) {
            printf("%d %d\n", i, cnt[i]);
        }
    }
    free(cnt);
    bs_destroy(p);
    fclose(fp);
    return 0;
}
