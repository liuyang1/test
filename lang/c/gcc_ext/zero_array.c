#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN(x, y) (x>y?y:x)

typedef struct {
    int refcnt;
    unsigned int len;
    // zero length array, which pointer to address after header
    unsigned char data[0];
} VArray;

#define CHUNK   2
#define GRP     16

unsigned int log16_ceil(unsigned int n)
{
    unsigned int i = -1;
    while (n) {
        i++;
        n >>= 4;
    }
    return i + 1;
}

/**
 * @brief beautify buffer showing
 */
int varray_show(VArray *p)
{
    printf("array: %p ref: %d len: %d\n", p, p->refcnt, p->len);
    unsigned int val = p->len;
    // smart width of cursor show
    unsigned int width = log16_ceil(val - 1);
    // printf("val: %d width: %d\n", val, width);
    unsigned int i;
    for (i = 0; i < p->len; i++) {
        if (i % GRP ==  0) {
            printf("0x%0*x: ", width, i);
        } else if (i % CHUNK == 0) {
            // only add delimiter whitespace between CHUNK
            // not add exter in head & tail
            printf(" ");
        }
        printf("%02x", p->data[i]);
        // right linefeed position
        // after one GROUP or after final line
        if ((i + 1) % GRP == 0 || (i + 1) == p->len) {
            printf("\n");
        }
    }
    return 0;
}

int main()
{
    char str[] = "hello, world\n";
    unsigned int len;
    for (len = 0; len < 18; len++) {
        VArray *pa = (VArray *)malloc(sizeof(VArray)
                                      + len * sizeof(unsigned char));
        strncpy(pa->data, str, MIN(len - 1, strlen(str)));
        pa->len = len;

        varray_show(pa);
        free(pa);
    }
    return 0;
}
