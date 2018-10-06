/** Humble Number
 *  ZJU 1095
 */
/** Key of this problem
 *  - use correct type: unsigned int
 *  - generate table
 *  This way is good, I don't remember it,
 *  ProjectEuler must have this kind of *  problem.
 */

#include <stdio.h>
#include <stdlib.h>

const char *suffix(int n) {
    int r2 = n % 100;
    int r = n % 10;
    if (r == 1 && r2 != 11) {
        return "st";
    } else if (r == 2 && r2 != 12) {
        return "nd";
    } else if (r == 3 && r2 != 13) {
        return "rd";
    } else {
        return "th";
    }
}

unsigned int min2(unsigned int a, unsigned int b) {
    return a < b ? a : b;
}

unsigned int min4(unsigned int a, unsigned int b,
                  unsigned int c, unsigned int d) {
    return min2(min2(a, b), min2(c, d));
}

void createHumble(int *a, int n) {
    unsigned int i0, i1, i2, i3, i, v = 1;
    a[0] = v;
    for (i = i0 = i1 = i2 = i3 = 0; i != n - 1; i++) {
        v = min4(2 * a[i0], 3 * a[i1], 5 * a[i2], 7 * a[i3]);
        i0 += v == 2 * a[i0];
        i1 += v == 3 * a[i1];
        i2 += v == 5 * a[i2];
        i3 += v == 7 * a[i3];
        a[i + 1] = v;
    }
}

int main() {
    const int m = 5842;
    int n, *a = malloc(sizeof(int) * m);
    createHumble(a, m);

    while (1) {
        if (scanf("%d", &n) == EOF || n == 0) {
            break;
        }
        printf("The %d%s humble number is %d.\n",
               n, suffix(n), a[n - 1]);
    }
    free(a);
    return 0;
}
