typedef enum {
    Nothing = 0,
    Just,
} MetaType;

typedef enum {
    VT_int = 0,
    VT_int2int,
} ValType;

typedef struct MaybeX {
    MetaType type;
    ValType vtype;
    union {
        int val;
        int (*fnc)(int);
    } x;
} MaybeX;

#define SHOWMAYBEINTLEN     20
bool isNothing(MaybeX v);
bool isJust(MaybeX v);
bool equal(MaybeX a, MaybeX b);

MaybeX JustInt(int v);
MaybeX JustFnc(int (*fnc)(int));
MaybeX NothingX();

char *show(char *s, MaybeX v);

MaybeX fmap(int (*func)(int), MaybeX v);
MaybeX Applicative(MaybeX a, MaybeX b);
