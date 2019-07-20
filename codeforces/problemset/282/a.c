#include <stdio.h>

int main() {
    unsigned int T;
    scanf("%u", &T);
    getchar();
    int x = 0;
    while (T--) {
        char a, b, c;
        a = getchar();
        b = getchar();
        c = getchar();
        getchar(); // eat newline
        if (a == '-' || b == '-' || c == '-') {
            x--;
        } else {
            x++;
        }
    }
    printf("%d\n", x);
    return 0;
}
