#include <stdio.h>
int func(int x) {
    switch(x) {
        case 0:
        case 1:
        case 2:
            printf("number x=%d\n", x);
            break;
        default:
            printf("default case x=%d\n", x);
            break;
        case 3:
            printf("number 3 x=%d\n", x);
            break;
    }
    return 0;
}

int main() {
    func(0);
    func(1);
    func(2);
    func(3);
    func(4);
    return 0;
}
