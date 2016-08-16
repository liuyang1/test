#include <stdio.h>
#include <stdbool.h>

int firstBadVersion(int n);

bool isBadVersion_sanity(int version) {
    return version >= 7;
}

int sanity() {
    int i;
    for (i = 7; i <= 20; i++) {
        printf("version=%d %d\n", i, firstBadVersion(i));
    }
    return 0;
}

bool isBadVersion_perf(int version) {
    return version >= 1702766719;
}

int perf() {
    int n = 2126753390;
    printf("version=%d %d\n", n, firstBadVersion(n));
    return 0;
}

bool isBadVersion_overflow(int n) {
    return n >= 2147483647;
}

int overflow() {
    int n = 2147483647;
    printf("version=%d %d\n", n, firstBadVersion(n));
    return 0;
}

// Forward declaration of isBadVersion API.
bool (*isBadVersion)(int version) = isBadVersion_perf;

int firstBadVersion(int n) {
    long long int minBad = n;
    long long int beg, end, mid;
    for (beg = 0, end = n; beg <= end;) {
        mid = (beg + end) / 2;
        if (isBadVersion(mid)) {
            if (minBad > mid) {
                minBad = mid;
            }
            end = mid - 1;
        } else {
            beg = mid + 1;
        }
    }
    return (int)minBad;
}

int main() {
    perf();
    // overflow();
    return 0;
}
