#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    struct timeval t;
    struct timezone tz;
    gettimeofday(&t, &tz);
    time_t now;
    time(&now);
    printf("time : %ld sec\n", now);
    printf("timeodday : %lu . %lu sec sizeof(sec)=%lu\n",
           t.tv_sec, t.tv_usec,
           sizeof(t.tv_sec));
    printf("timezone : %u min\n", tz.tz_minuteswest);

    // convert time to 32bit test.
    unsigned long long ms = t.tv_sec * 1000 + t.tv_usec / 1000;
    printf("ms: %llu to U32 %u dt=%u\n",
           ms, (unsigned int)(ms), (unsigned int)(-1) - (unsigned int)(ms));
    unsigned int dt = (unsigned int)(-1) - (unsigned int)(ms);
    printf("dt=%u %d\n", dt, dt / 1000 / 60 / 60 / 24);
    return 0;
}
