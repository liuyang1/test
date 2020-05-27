/** codeforces.com/problemset/problem/1360/A */
#include <stdio.h>

// gcc extension style
#define SWAP(a, b)  { typeof(a) t_ = a; a = b; b = t_; }
// generic style
#define SWAPT(a, b, type) { type t_ = a; a = b; b = t_; }
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// #define INFO(...) fprintf(stderr, __VA_ARGS__)
#define INFO(...)

int main() {
    int t = 0, a, b, c;
    for (scanf("%d", &t); t != 0; t--) {
        scanf("%d%d", &a, &b);
        if (a > b) {
            SWAPT(a, b, int);
        }
        // a <= b
        c = MAX(2 * a, b);
        INFO("a=%d b=%d c=%d\n", a, b, c);
        printf("%d\n", c * c);
    }
    return 0;
}
