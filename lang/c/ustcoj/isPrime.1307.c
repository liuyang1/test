#include <stdio.h>
#include <stdbool.h>

bool isPrime(int a) {
    if (a == 1) {
        return false;
    }
    int i;
    for (i = 2; i * i <= a; i++) {
        if (a % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int casenum;
    scanf("%d", &casenum);
    while (casenum--) {
        int n;
        scanf("%d", &n);
        printf("%s\n", isPrime(n) ? "Yes" : "No");
    }
    return 0;
}
