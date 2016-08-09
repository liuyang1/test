#include <stdio.h>
int main() {
    int foo = 7;
    const int bar = 4;
    switch(foo) {
        case bar:
            printf("This is case 0\n");
    }
    return 0;
}
