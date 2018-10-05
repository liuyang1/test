/**
 * The key of this issue is must consider input large as 10 ** 10
 * So we have to use UNSIGNED LONG LONG INT type EVERY WHERE
 * My failure is missing change type in digitSum function
 * If I check it again through whole source code, I would find out it
 * But I lost my patience
 *
 * CALM DOWN next time!!!
 */

/** Solution:
 *  one number is 9999...9, other one is left part.
 */
#include <stdio.h>
#include <stdlib.h>

int digitSum(long long int n) {
    int s = 0;
    while (n != 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

// slow & correct solution
int sln(int n) {
    int i, m = 0;
    for (i = 0; i <= n / 2; i++) {
        int v = digitSum(i) + digitSum(n - i);
        if (v > m) {
            m = v;
        }
    }
    return m;
}

int cntdigit(long long int n) {
    int c = 0;
    while (n != 0) {
        c++;
        n /= 10;
    }
    return c;
}

int sln1(long long int n) {
    int c = cntdigit(n);
    long long int i, v = 1;
    for (i = 0; i != c; i++) {
        v *= 10;
    }
    v /= 10;
    v--;
    return digitSum(v) + digitSum(n - v);
}

void unit(unsigned long long int n) {
    int r1 = sln1(n);
    // int r2= sln(i);
    int i, m = 0;
    for (i = n / 2; i != 0; i--) {
        int v = digitSum(i) + digitSum(n - i);
        if (v > m) {
            m = v;
            if (m > r1) {
                printf("solution is wrong for now\n");
                exit(-1);
            }
        }
    }
    int r2 = m;
    // printf("%lld: %d %d\n", n, r1, r2);
    if (r1 != r2) {
        exit(-1);
    }

}

void run(long long int n) {
    int r = sln1(n);
    // printf("%lld: %d\n", n, r);
}

void test() {
    unit(9992); // 56
    unit(99992); // 74
    unit(999992); // 92
    unit(9999992); // 110
    // unit(99999992); // 128
    // unit(999999992); // 146
    // unit(9999999992);
    run(999999999992ull);
    return;
    run(1); // 1
    run(1000); // 3
    run(1000000); // 6
    run(1000000000); // 9
    run(10000000000ull); // 10
    run(1000000000000ull); // 12
    run(999999999992ull);
    // return;
    int i;
    for (i = 1; i != 1000 * 10 * 2; i++) {
        unit(i);
    }
}

int main() {
    // test();
    // return 0;
    long long int n;
    scanf("%I64lld", &n);
    // printf("%lld\n", n);
    int r = sln1(n);
    printf("%d\n", r);
    return 0;
}
