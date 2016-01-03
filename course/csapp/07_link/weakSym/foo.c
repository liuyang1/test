int x;
// weak symbol, even not setting `extern`, but will search other strong symbol,
// so it's override by `x` in main.c
// when assign `x` in `f` func, it will assign to `x` in main.c

void f(void) {
    x = 2;
}
