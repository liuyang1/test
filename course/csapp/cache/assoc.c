#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SEC2NSEC(x) (x * 1000 * 1000 * 1000)

// libbsd have this
void timespecsub(struct timespec *start, struct timespec *stop, struct timespec *ret) {
    bool f = stop->tv_nsec < start->tv_nsec;
    ret->tv_sec = stop->tv_sec - start->tv_sec - f;
    ret->tv_nsec = stop->tv_nsec + SEC2NSEC(f) - start->tv_nsec;
}


int main() {
    size_t step;
    for (step = 16; step != 256 * 2 + 1; step++) {
        const size_t n = 1024 * 1024 * 8;
        int *a = malloc(sizeof(int) * n);
        size_t i;
        for (i = 0; i != n; i++) {
            a[i] = random();
        }

        struct timespec ts0, ts1, t;
        clock_gettime(CLOCK_MONOTONIC, &ts0);
        // TIC
        size_t sum = 0, cnt = 0;
        for (i = 0; i < n; i+= step) {
            sum += a[i]++;
            cnt++;
        }
        // TOC
        clock_gettime(CLOCK_MONOTONIC, &ts1);

        timespecsub(&ts0, &ts1, &t);
        printf("step=%zu time=%lu.%lus cnt=%zu avg=%f\n",
               step, t.tv_sec, t.tv_nsec, cnt, (t.tv_nsec + SEC2NSEC(t.tv_sec)) / (cnt + 0.));

        free(a);
    }

    return 0;
}
