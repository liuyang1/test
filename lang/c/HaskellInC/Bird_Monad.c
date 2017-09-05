#include <stdio.h>
#include <stdlib.h>
typedef int Birds;

typedef enum {
    Just = 0,
    Nothing = 1,
} MaybeT;

typedef struct {
    Birds left;
    Birds right;
} Pole;

typedef struct {
    MaybeT maybe;
    Pole pole;
} MaybePole;

MaybePole JustPole(Pole pole) {
    MaybePole r = {.maybe = Just, .pole = pole};
    return r;
}

MaybePole NothingPole() {
    MaybePole r = {.maybe = Nothing};
    return r;
}

/* basic function for Maybe monad */
MaybePole returnMaybe(Pole pole) {
    return JustPole(pole);
}

MaybePole bind(MaybePole mpole, MaybePole(*func)(Pole pole)) {
    if (mpole.maybe == Nothing) {
        return NothingPole();
    }
    return func(mpole.pole);
}

void dispMaybePole(MaybePole mpole) {
    if (mpole.maybe == Nothing) {
        printf("Nothing\n");
    } else {
        printf("Just (%d, %d)\n", mpole.pole.left, mpole.pole.right);
    }
}

MaybePole check(Pole pole) {
    if (abs(pole.right - pole.right) < 4) {
        return JustPole(pole);
    }
    return NothingPole();
}

MaybePole landLeft(Birds n, Pole pole) {
    Pole r = {.left = pole.left + n, .right = pole.right};
    return check(r);
}

MaybePole landRight(Birds n, Pole pole) {
    Pole r = {.left = pole.left, .right = pole.right + n};
    return check(r);
}

// sorry, we don't have currying in C.
MaybePole landLeft1(Pole pole) {
    return landLeft(1, pole);
}

MaybePole landRight1(Pole pole) {
    return landRight(1, pole);
}

MaybePole landLeft2(Pole pole) {
    return landLeft(2, pole);
}

MaybePole banana(Pole pole) {
    return NothingPole();
}

void test0() {
    Pole initial = {.left = 0, .right = 0};
    MaybePole ret = bind(bind(bind(returnMaybe(initial),
                                   landLeft1),
                              landRight1),
                         landLeft2);
    dispMaybePole(ret);
}
void test2() {
    Pole initial = {.left = 0, .right = 0};
    MaybePole ret = bind(bind(bind(returnMaybe(initial),
                                   landLeft1),
                              banana),
                         landLeft2);
    dispMaybePole(ret);
}
int main() {
    test0();
    test2();
    return 0;
}
