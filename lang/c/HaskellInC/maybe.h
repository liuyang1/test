typedef enum {
    Nothing = 0,
    Just,
    Left,
    Right,
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

/**
 * Either String Int
 */
typedef struct EitherX {
    MetaType type;
    ValType vtype;
    union {
        int val;
        char *msg;
    } x;
} EitherX;

#define SHOWMAYBEINTLEN     256
bool isNothing(MaybeX v);
bool isJust(MaybeX v);
bool equal(MaybeX a, MaybeX b);

MaybeX JustInt(int v);
MaybeX JustFnc(int (*fnc)(int));
MaybeX NothingX();

char *show(char *s, MaybeX v);

MaybeX fmap(int (*func)(int), MaybeX v);
MaybeX Applicative(MaybeX a, MaybeX b);

bool isLeft(EitherX v);
bool isRight(EitherX v);
bool equalEither(EitherX v);


