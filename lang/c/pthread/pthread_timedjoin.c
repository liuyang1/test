#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void *func(void *pdt)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    int dt = *(int *)pdt;
    printf("main: long time function, wait dt=%u, send sem\n", dt);
    sleep(dt);
    printf("main: long time function end\n");
    return 0;
}

#define NSEC_PER_SEC_U64 (1*1000ull*1000*1000)

struct timespec afterNanoSec(unsigned long long nanosec)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    nanosec += now.tv_nsec;
    now.tv_sec += nanosec / NSEC_PER_SEC_U64;
    now.tv_nsec = nanosec % NSEC_PER_SEC_U64;
    return now;
}

struct timespec afterSec(unsigned long long sec)
{
    return afterNanoSec(NSEC_PER_SEC_U64 * sec);
}

struct timespec now()
{
    return afterNanoSec(0);
}

int main() {

    pthread_t thread1;
    while (1) {
        int dt;
        printf("join wait 2 sec, input function need time(>60, exit):");
        scanf("%d", &dt);
        if (dt > 60) {
            break;
        }
        {
            struct timespec ts = now();
            printf("current time: %u sec %llu ns\n", ts.tv_sec, ts.tv_nsec);

            pthread_create(&thread1, NULL, func, &dt);

            void *retval;
            ts = afterSec(2);
            int ret = pthread_timedjoin_np(thread1, &retval, &ts);

            ts = now();
            printf("current time: %u sec %llu ns\n", ts.tv_sec, ts.tv_nsec);
            if (ret == ETIMEDOUT) {
                printf("thread cannot join as timeout\n");
                printf("thread cancel\n");
                ret = pthread_cancel(thread1);
                if (ret != 0) {
                    printf("pthread_cancel ret=%x\n", ret);
                }
                ret = pthread_join(thread1, &retval);
            }
            if (retval == PTHREAD_CANCELED) {
                printf("thread join with cancel\n");
            } else {
                printf("thread join with retval=%p ret=%x\n", retval, ret);
            }

        }
    }

    return 0;
}
