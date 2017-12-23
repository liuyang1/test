/* 903A
 * http://codeforces.com/problemset/problem/903/A
 * find out number cannot write this format:
 * X = 3*a + 7*b, X C- Z+
 * only {1, 2, 3, 4, 5, 8, 11} cannot write to this format
 * O(1)
 */
#include <stdio.h>
#include <stdbool.h>

bool predBuy(int n) {
    const int tbl[] = {1, 2, 4, 5, 8, 11};
    int i;
    for (i = 0; i != sizeof(tbl) / sizeof(tbl[0]); i++) {
        if (n == tbl[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    scanf("%d", &n);
    int i;
    for (i = 0; i != n; i++) {
        int m;
        scanf("%d", &m);
        printf("%s\n", predBuy(m) ? "YES" : "NO");
    }
}
