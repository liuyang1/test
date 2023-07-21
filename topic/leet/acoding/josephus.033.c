/** josephus problem with recursion
 * J(n,k)=(J(n-1,k)+k-1)%n+1
 * J(n,k), first cycle is remove the kth prison, to all remain prison's number is shifted k-1.
 */
#include <stdio.h>

// solution
int josephus(int n, int k) {
    if (n == 1) {
        return 1;
    } else {
        return (josephus(n-1, k) + k - 1) % n + 1;
    }
}
// solution end

int unit(int n, int k, int e) {
    int j = josephus(n, k);
    printf("%d,%d = %d\n", n, k, j);
    if (j != e) {
        printf("fail e=%d\n", e);
    }
    return 0;
}

int main() {
    unit(5, 2, 3);
    unit(7, 3, 4);
    return 0;
}
