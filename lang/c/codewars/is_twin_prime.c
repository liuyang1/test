#include <stdio.h>
#include <stdbool.h>

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2, step = 1; i * i <= n;) {
        if (n % i == 0) {
            return false;
        }
        i += step;
        step = 2;
    }
    return true;
}
bool is_twin_prime(int n) {
    return is_prime(n) && (is_prime(n - 2) || is_prime(n + 2));
}

int test_is_prime() {
    for (int i = 0; i != 100; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}

int test_is_twin_prime() {
    for (int i = 0; i != 100; i++) {
        if (is_twin_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}

int main() {
    test_is_prime();
    test_is_twin_prime();
    return 0;
}
