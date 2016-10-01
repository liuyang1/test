#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "maybe.h"

bool isNothing(MaybeX v) {
    return v.type == Nothing;
}

bool isJust(MaybeX v) {
    return !isNothing(v);
}

bool isJustInt(MaybeX v) {
    return isJust(v) && v.vtype == VT_int;
}

bool isJustFnc(MaybeX v) {
    return isJust(v) && v.vtype == VT_int2int;
}

bool equal(MaybeX a, MaybeX b) {
    if (isNothing(a) && isNothing(b)) {
        return true;
    }
    if (isJust(a) && isJust(b) && a.vtype == b.vtype) {
        switch (a.vtype) {
            case VT_int:
                return a.x.val == b.x.val;
            case VT_int2int:
                return a.x.fnc == b.x.fnc;
            default:
                assert(0);
        }
    }
    return false;
}

MaybeX JustInt(int v) {
    MaybeX r;
    r.type = Just;
    r.vtype = VT_int;
    r.x.val = v;
    return r;
}

MaybeX JustFnc(int (*fnc)(int)) {
    MaybeX r;
    r.type = Just;
    r.vtype = VT_int2int;
    r.x.fnc = fnc;
    return r;
}

MaybeX NothingX() {
    MaybeX r;
    r.type = Nothing;
    return r;
}

/** TODO: function compose in C ???
 */
char *show(char *s, MaybeX v) {
    if (isNothing(v)) {
        snprintf(s, SHOWMAYBEINTLEN, "Nothing");
    } else {
        switch(v.vtype) {
            case VT_int:
                snprintf(s, SHOWMAYBEINTLEN, "Just %d", v.x.val);
                return s;
            case VT_int2int:
                snprintf(s, SHOWMAYBEINTLEN, "Just <func %p>", v.x.fnc);
                return s;
            default:
                assert(0);
        }
    }
    return s;
}

/**
 * fmap :: (a -> b) -> f a -> f b
 * fmap :: (int -> int) -> Maybe int -> Maybe Int
 *
 * instance Functor Maybe where
 *     fmap f (Just x) = Just (f x)
 *     fmap f Nothing = Nothing
 */
MaybeX fmap(int (*func)(int), MaybeX v) {
    if (isJustInt(v)) {
        v.x.val = func(v.x.val);
        return v;
    }
    return NothingX();
}

/**
 * instance Applicative Maybe where
 *     pure = Just
 *     Nothing <*> _ = Nothing
 *     (Just f) <*> something = fmap f something
 */
MaybeX Applicative(MaybeX a, MaybeX b) {
    if (isNothing(a) || isNothing(b)) {
        return NothingX();
    }
    if (isJustFnc(a) && isJustInt(b)) {
        return JustInt(a.x.fnc(b.x.val));
    }
    char sa[SHOWMAYBEINTLEN], sb[SHOWMAYBEINTLEN];
    printf("cannot support Applicative(%s, %s)\n", show(sa, a), show(sb, b));
    return NothingX();
}
