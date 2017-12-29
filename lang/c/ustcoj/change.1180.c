#include <stdio.h>
#include <stdlib.h>

/* X = a1 - b1 + 5(a2 - b2) + 10(a3 - b3) + 20(a4 - b4) + 100(a5 - b5)
 * MIN(sum(|ai| + |bi|))
 * greedy
 */
const int money[] = {1, 5, 10, 20, 50, 100};

int cntChange(int n) {
    int cnt = 0;
    while (n != 0) {
        int min, i, diff0, diff1;
        for (i = 0, min = 200; i != sizeof(money) / sizeof(money[0]); i++) {
            diff0 = n - money[i];
            diff1 = n + money[i];
            if (abs(diff1) < abs(diff0)) {
                diff0 = diff1;
            }
            if (abs(diff0) < abs(min)) {
                min = diff0;
            }
            printf("try %d -> %d\n", money[i], diff0);
        }
        n = min;
        printf("%d\n", n);
        cnt++;
    }
    return cnt;
}

int main() {
#if 1
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        printf("%d\n", cntChange(n));
    }
#else
    int i;
    for (i = 0; i != 100; i++) {
        if (cntChange(i) == 5) {
            printf("%d: %d\n", i, cntChange(i));
        }
    }
#endif
    return 0;
}
