#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (a < b ? a : b)

double myabs(double x) {
    return x > 0 ? x : -x;
}

double ratioMin(int sec, int min) {
    return sec / 10.0 + min * 6.0;
}

double ratioHour(int sec, int min, int hour) {
    hour %= 12;
    return ratioMin(sec, min) / 12.0 + hour * 30.0;
}

double ratioAbsDiff(double a, double b) {
    double d0 = myabs(a - b);
    double d1 = 360.0 - d0;
    return MIN(d0, d1);
}

int main() {
    int casenum, i;
    scanf("%d", &casenum);
    for (i = 0; i != casenum; i++) {
        int sec, min, hour;
        scanf("%d%d%d", &hour, &min, &sec);
        double ratio = ratioAbsDiff(ratioMin(sec, min), ratioHour(sec, min, hour));
        printf("%d\n", (int)(ratio));
    }
    return 0;
}
