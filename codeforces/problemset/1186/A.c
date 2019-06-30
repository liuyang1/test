#include <stdio.h>

int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    printf("%s\n",
           n <= m && n <= k ? "Yes" : "No");
    return 0;
}
