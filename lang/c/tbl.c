#include <stdio.h>
#include <string.h>

typedef struct {
    int val;
    char *str;
} EnumStrItem;

#define PREFIX_VAL0     0
#define PREFIX_VAL1     1
#define PREFIX_MAX      (-1)
#define DECLARE_PREFIX_MAP(x) { .val = PREFIX_##x, .str = #x }
EnumStrItem PrefixIdMap[] = {
    DECLARE_PREFIX_MAP(VAL0),
    DECLARE_PREFIX_MAP(VAL1),
    DECLARE_PREFIX_MAP(MAX),
};

static char * enum2str(EnumStrItem tbl[], int val)
{
    int i = 0;
    for (i = 0; tbl[i].val != PREFIX_MAX; i++) {
        if (tbl[i].val == i) {
            return tbl[i].str;
        }
    }
    return NULL;
}
static int str2enum(EnumStrItem tbl[], char *s)
{
    int i = 0;
    for (i = 0; tbl[i].val != PREFIX_MAX; i++) {
        if (strcmp(s, tbl[i].str) == 0) {
            return tbl[i].val;
        }
    }
    return -1;
}
static char *PrefixId2Name(int val)
{
    return enum2str(PrefixIdMap, val);
}
static int PrefixName2Id(char *name)
{
    return str2enum(PrefixIdMap, name);
}

// cannot pass array's length, after transfter to pointer
// or indicate is by param: EnumStrItem tbl[10], that's meaningless.
// so we use (-1) as MAX to GUARD.
int main(void)
{
    printf("VAL1 -> %d\n", PrefixName2Id("VAL1"));
    printf("1 -> %s\n", PrefixId2Name(PREFIX_VAL1));
    return 0;
}
