#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    time_t uninit;
    time_t now;
    struct tm *timenow;

    time(&now);
    printf("%d\n", now);

    timenow = localtime(&now);
    printf("local time is %d\t%s\n", now, asctime(timenow));
    // ctime = localtime(asctime(struct tim*));
    printf("ctime %s\n", ctime(&now));

    printf("ctime uninit: %s\n", ctime(&uninit));

    char buf[40];
    strftime(buf, 40, "%H:%M:%S %Y-%m-%d", timenow);
    printf("strftime: %s\n", buf);
    return 0;
}
