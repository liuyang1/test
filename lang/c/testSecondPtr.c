#include <stdio.h>

#define BLOCK_BITS 4
#define BLOCK_SIZE (1<<BLOCK_BITS)
#define BLOCK_MASK (BLOCK_SIZE - 1)
#define MAX         0x80
#define BLOCKS      (MAX >> BLOCK_BITS)
#define BLOCK(v)    ((v) >> BLOCK_BITS)
#define INDEX(v)    ((v) & BLOCK_MASK)

#define printMacro(m) {printf("%s: 0x%02x %d\n", #m, m, m);}

void testMacro()
{
    printMacro(BLOCK_BITS);
    printMacro(BLOCK_SIZE);
    printMacro(BLOCK_MASK);
    printMacro(MAX);
    printMacro(BLOCKS);
}

void insert(int v)
{
    static int nr = 0;
    int vv;
    printf("insert %d = %d * %d + %d\n", v, BLOCK(v), BLOCK_SIZE, INDEX(v));
    if (v > nr) {
        vv = (v + BLOCK_SIZE) & ~BLOCK_MASK;
        // equal::
        // vv = (v / BLOCK_SIZE + 1) * BLOCK_SIZE;
        printf("vv %d\n", vv);
        int i;
        for (i = 0; i < BLOCK(vv); i++) {
            printf("i: %d\n", i);
        }
        nr = vv;
    } else {
        printf("directly exit\n");
    }
}
int main(int argc, char *argv[])
{
    testMacro();
    printf("----as macro define as above, test as below----\n");
    insert(3);
    insert(16);
    insert(40);
    insert(30);
    insert(100);
    insert(40);
    return 0;
}
