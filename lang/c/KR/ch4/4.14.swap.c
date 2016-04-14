#include "../kr.h"

#define swap(T, x, y) {T t = x; x = y; y = t;}

int main() {
    int x = 1, y = 2;
    printf("before swap x=%d y=%d\n", x, y);
    swap(int, x, y);
    printf("after  swap x=%d y=%d\n", x, y);

    double dx = 1, dy = 2;
    printf("before swap dx=%f dy=%f\n", dx, dy);
    swap(double, dx, dy);
    printf("after  swap dx=%f dy=%f\n", dx, dy);
    return 0;
}
